package com.brackeen.javagamebook.tilegame;

import java.awt.*;
import java.util.Scanner;
import java.awt.event.KeyEvent;
import java.util.Iterator;

import javax.sound.midi.Sequence;
import javax.sound.midi.Sequencer;
import javax.sound.sampled.AudioFormat;

import com.brackeen.javagamebook.graphics.*;
import com.brackeen.javagamebook.sound.*;
import com.brackeen.javagamebook.input.*;
import com.brackeen.javagamebook.test.GameCore;
import com.brackeen.javagamebook.tilegame.sprites.*;

/**
    GameManager manages all parts of the game.
*/
public class GameManager extends GameCore {

	static String filename;
	static boolean input_option = false;
    public static void main(String[] args) {
    	Scanner input = new Scanner(System.in);
    	System.out.println("Choose map to load:");
    	while(true) {
    		System.out.println("A. Default map.");
    		System.out.println("B. Custom map.");
    		String choice = input.nextLine();
    		if (choice.equals("A")) {
    			input_option = false;
    			break;
    		}
    		else if (choice.equals("B")) {
    			input_option = true;
    			break;
    		}
    		else {
    			System.out.println("Please enter a valid option.");
    		}
    	}
        new GameManager().run();
    }

    // uncompressed, 44100Hz, 16-bit, mono, signed, little-endian
    private static final AudioFormat PLAYBACK_FORMAT =
        new AudioFormat(44100, 16, 1, true, false);

    private static final int DRUM_TRACK = 1;

    public static final float GRAVITY = 0.002f;

    private Point pointCache = new Point();
    private TileMap map;
    private MidiPlayer midiPlayer;
    private SoundManager soundManager;
    private ResourceManager resourceManager;
    private Sound prizeSound;
    private Sound boopSound;
    private Sound shootSound;	//add shooting sound
    private Sound bugshoot;
    private InputManager inputManager;
    private TileMapRenderer renderer;

    private GameAction moveLeft;
    private GameAction moveRight;
    private GameAction jump;
    private GameAction exit;
    private GameAction shoot;	//add shoooing action
    private GameAction down;
    /*private GameAction down; //add down action */


    public void init() {
        super.init();

        // set up input manager
        initInput();

        // start resource manager
        resourceManager = new ResourceManager(
        screen.getFullScreenWindow().getGraphicsConfiguration());

        // load resources
        renderer = new TileMapRenderer();
        renderer.setBackground(
            resourceManager.loadImage("background.png"));

        // load first map
        if (input_option == true) {
        	map = resourceManager.loadcustomMap();
        }
        else{
        	map = resourceManager.loadNextMap();
        }

        // load sounds
        soundManager = new SoundManager(PLAYBACK_FORMAT);
        prizeSound = soundManager.getSound("sounds/prize.wav");
        boopSound = soundManager.getSound("sounds/boop2.wav");
        shootSound = soundManager.getSound("sounds/shoot.wav");
        bugshoot = soundManager.getSound("sounds/shoot.wav");

        // start music
        midiPlayer = new MidiPlayer();
        Sequence sequence =
            midiPlayer.getSequence("sounds/music.midi");
        midiPlayer.play(sequence, true);
        toggleDrumPlayback();
    }


    /**
        Closes any resurces used by the GameManager.
    */
    public void stop() {
        super.stop();
        midiPlayer.close();
        soundManager.close();
    }


    private void initInput() {
        moveLeft = new GameAction("moveLeft");
        moveRight = new GameAction("moveRight");
        jump = new GameAction("jump",
            GameAction.DETECT_INITAL_PRESS_ONLY);
        exit = new GameAction("exit",
            GameAction.DETECT_INITAL_PRESS_ONLY);
        shoot = new GameAction("shoot");
        down = new GameAction("down");

        inputManager = new InputManager(
            screen.getFullScreenWindow());
        inputManager.setCursor(InputManager.INVISIBLE_CURSOR);

        inputManager.mapToKey(moveLeft, KeyEvent.VK_LEFT);
        inputManager.mapToKey(moveRight, KeyEvent.VK_RIGHT);
        inputManager.mapToKey(jump, KeyEvent.VK_UP);
        inputManager.mapToKey(down, KeyEvent.VK_DOWN);
        inputManager.mapToKey(exit, KeyEvent.VK_ESCAPE);
        inputManager.mapToKey(shoot, KeyEvent.VK_S);
    }

    private long currtime = System.currentTimeMillis();
    private int shoot_count = 0;
    private boolean reload = false;
    private boolean shooting = false;
    
    private void checkInput(long elapsedTime) {

        if (exit.isPressed()) {
            stop();
        }

        Player player = (Player)map.getPlayer();
        if (player.isAlive()) {
            float velocityX = 0;
            float velocityY = 0;
            if (moveLeft.isPressed()) {
                velocityX-=player.getMaxSpeed();
            }
            if (moveRight.isPressed()) {
                velocityX+=player.getMaxSpeed();
            }
            if (jump.isPressed()) {
                player.jump(false);
            }
            if (down.isPressed()) {
            	velocityY+=player.getMaxSpeed();
            	player.setVelocityY(velocityY);
            }
        player.setVelocityX(velocityX);
            
            /*if (down.isPressed()) {
            	velocityY+=player.getMaxSpeed();
            	player.setVelocityY(velocityY);
            }*/
            if (shoot.isPressed()) {
            	if (shoot_count == 0) {
            		if (reload) {
            			if (System.currentTimeMillis() - currtime >= 1000) {
            				reload = false;
            				shooting = true;
            				shoot_count = shoot_count + 1;
            				currtime = System.currentTimeMillis();
            			}
            		}
            		else {
            			shooting = true;
            			shoot_count = shoot_count + 1;
            			currtime = System.currentTimeMillis();
            		}
            	}
            	else if (shoot_count == 1) {
            		if (System.currentTimeMillis() - currtime >= 1000) {
            			shooting = true;
            			shoot_count = shoot_count + 1;
            			currtime = System.currentTimeMillis();
            		}
            	}
            	else if (shoot_count <= 10) {
            		if (System.currentTimeMillis() - currtime >= 400){
            			shooting = true;
            			shoot_count = shoot_count + 1;
            			currtime = System.currentTimeMillis();
            		}
            	}
            	else {
            		reload = true;
            		if (System.currentTimeMillis() - currtime >= 1000){
            			shooting = true;
            			reload = false;
            			shoot_count = 2;
            			currtime = System.currentTimeMillis();
            		}
            	}
            }
            else{
            	shoot_count = 0;
            }
        }

    }


    public void draw(Graphics2D g) {
        renderer.draw(g, map,
            screen.getWidth(), screen.getHeight());
        //display Health and score
        Player charr = (Player)map.getPlayer();
        g.setColor(Color.black);
        g.setFont(new Font("TimesRoman", 0, 30));
        g.drawString("Health:" + charr.getHealth(), 50, 50);
        //g.drawString("Score:" + charr.getScore(), 500, 50);
        if (reload) {
        	g.setColor(Color.red);
        	g.setFont(new Font("TimesRoman", 0, 70));
        	g.drawString("YO, SLOW DOWN", 80, 300);
        }
        if (charr.getHealth() == 0) {
        	g.setColor(Color.red);
        	g.setFont(new Font("TimesRoman", 0, 100));
        	g.drawString("GAME OVER", 80, 300);
        }
        if (charr.invincible == true) {
        	g.setColor(Color.red);
        	g.setFont(new Font("TimesRoman", 0, 30));
        	g.drawString("GOGOGO", 500, 50);
        }
        
    }
    


    /**
        Gets the current map.
    */
    public TileMap getMap() {
        return map;
    }


    /**
        Turns on/off drum playback in the midi music (track 1).
    */
    public void toggleDrumPlayback() {
        Sequencer sequencer = midiPlayer.getSequencer();
        if (sequencer != null) {
            sequencer.setTrackMute(DRUM_TRACK,
                !sequencer.getTrackMute(DRUM_TRACK));
        }
    }


    /**
        Gets the tile that a Sprites collides with. Only the
        Sprite's X or Y should be changed, not both. Returns null
        if no collision is detected.
    */
    public Point getTileCollision(Sprite sprite,
        float newX, float newY)
    {
        float fromX = Math.min(sprite.getX(), newX);
        float fromY = Math.min(sprite.getY(), newY);
        float toX = Math.max(sprite.getX(), newX);
        float toY = Math.max(sprite.getY(), newY);

        // get the tile locations
        int fromTileX = TileMapRenderer.pixelsToTiles(fromX);
        int fromTileY = TileMapRenderer.pixelsToTiles(fromY);
        int toTileX = TileMapRenderer.pixelsToTiles(
            toX + sprite.getWidth() - 1);
        int toTileY = TileMapRenderer.pixelsToTiles(
            toY + sprite.getHeight() - 1);

        // check each tile for a collision
        for (int x=fromTileX; x<=toTileX; x++) {
            for (int y=fromTileY; y<=toTileY; y++) {
                if (x < 0 || x >= map.getWidth() ||
                    map.getTile(x, y) != null)
                {
                    // collision found, return the tile
                    pointCache.setLocation(x, y);
                    return pointCache;
                }
            }
        }

        // no collision found
        return null;
    }


    /**
        Checks if two Sprites collide with one another. Returns
        false if the two Sprites are the same. Returns false if
        one of the Sprites is a Creature that is not alive.
    */
    public boolean isCollision(Sprite s1, Sprite s2) {
        // if the Sprites are the same, return false
        if (s1 == s2) {
            return false;
        }

        // if one of the Sprites is a dead Creature, return false
        if (s1 instanceof Creature && !((Creature)s1).isAlive()) {
            return false;
        }
        if (s2 instanceof Creature && !((Creature)s2).isAlive()) {
            return false;
        }

        // get the pixel location of the Sprites
        int s1x = Math.round(s1.getX());
        int s1y = Math.round(s1.getY());
        int s2x = Math.round(s2.getX());
        int s2y = Math.round(s2.getY());

        // check if the two sprites' boundaries intersect
        return (s1x < s2x + s2.getWidth() &&
            s2x < s1x + s1.getWidth() &&
            s1y < s2y + s2.getHeight() &&
            s2y < s1y + s1.getHeight());
    }


    /**
        Gets the Sprite that collides with the specified Sprite,
        or null if no Sprite collides with the specified Sprite.
    */
    public Sprite getSpriteCollision(Sprite sprite) {

        // run through the list of Sprites
        Iterator i = map.getSprites();
        while (i.hasNext()) {
            Sprite otherSprite = (Sprite)i.next();
            if (isCollision(sprite, otherSprite)) {
                // collision found, return the Sprite
                return otherSprite;
            }
        }

        // no collision found
        return null;
    }


    /**
        Updates Animation, position, and velocity of all Sprites
        in the current map.
    */
    public void update(long elapsedTime) {
        Creature player = (Creature)map.getPlayer();
        Sprite Fire = (Sprite) resourceManager.getFire().clone();
        Sprite creaturefire;

        // player is dead! start map over
        if (player.getState() == Creature.STATE_DEAD) {
            map = resourceManager.reloadMap();
            return;
        }

        // get keyboard/mouse input
        checkInput(elapsedTime);

        // update player
        updateCreature(player, elapsedTime);
        player.update(elapsedTime);

        // update other sprites
        Iterator i = map.getSprites();
        while (i.hasNext()) {
            Sprite sprite = (Sprite)i.next();
            if (sprite instanceof Creature) {
                Creature creature = (Creature)sprite;
                if (creature.getState() == Creature.STATE_DEAD) {
                    i.remove();
                }
                else {
                    creaturefire = updateCreature(creature, elapsedTime);
                    if (creaturefire != null) {
                    	map.addCreatureFire(creaturefire);
                    }
                }
            }
            // normal update
            sprite.update(elapsedTime);
        }
        //update bullet
        if (shooting) {
        	shooting = false;
        	Fire.setY(player.getY() +20);
        	Fire.setVelocityY(0);
        	if (!player.to_R) {
        		Fire.setVelocityX(-1.0f);
        		Fire.setX(player.getX());
        	}
        	else {
        		Fire.setVelocityX(1.0f);
        		Fire.setX(player.getX());
        	}
        	map.addSprite(Fire);
        	soundManager.play(shootSound);
        }
    }


    /**
        Updates the creature, applying gravity for creatures that
        aren't flying, and checks collisions.
    */
    private CreatureFire updateCreature(Creature creature,
        long elapsedTime)
    {

        // apply gravity
        if (!creature.isFlying()) {
        	if (creature instanceof Fire || creature instanceof CreatureFire){
        		//no gravity for bullets
        	}
        	else{
        		creature.setVelocityY(creature.getVelocityY() +
        				GRAVITY * elapsedTime);
        	}
        }

        // change x
        float dx = creature.getVelocityX();
        float oldX = creature.getX();
        float newX = oldX + dx * elapsedTime;
        if (creature instanceof Fire) {
        	if (creature.walk_accumulation(Math.abs(dx)) < creature.max_length){
        		creature.setX(newX);
        	}
        	else{
        		creature.setState(Creature.STATE_DEAD);
        		creature.length_walk = 0;
        	}
        	checkshoot((Fire)creature, (Player)map.getPlayer());
        	return null;
        }
        if (creature instanceof CreatureFire) {
        	if (creature.walk_accumulation(Math.abs(dx)) < creature.creature_max_length){
        		creature.setX(newX);
        	}
        	else{
        		creature.setState(Creature.STATE_DEAD);
        		creature.length_walk = 0;
        	}
        	return null;
        }
        Point tile =
            getTileCollision(creature, newX, creature.getY());
        if (tile == null) {
            creature.setX(newX);
        }
        else {
            // line up with the tile boundary
            if (dx > 0) {
                creature.setX(
                    TileMapRenderer.tilesToPixels(tile.x) -
                    creature.getWidth());
            }
            else if (dx < 0) {
                creature.setX(
                    TileMapRenderer.tilesToPixels(tile.x + 1));
            }
            creature.collideHorizontal();
        }
        if (creature instanceof Player) {
            checkPlayerCollision((Player)creature, false);
        }

        // change y
        float dy = creature.getVelocityY();
        float oldY = creature.getY();
        float newY = oldY + dy * elapsedTime;
        tile = getTileCollision(creature, creature.getX(), newY);
        if (tile == null) {
            creature.setY(newY);
        }
        else {
            // line up with the tile boundary
            if (dy > 0) {
                creature.setY(
                    TileMapRenderer.tilesToPixels(tile.y) -
                    creature.getHeight());
            }
            else if (dy < 0) {
                creature.setY(
                    TileMapRenderer.tilesToPixels(tile.y + 1));
            }
            creature.collideVertical();
        }
        if (creature instanceof Player) {
            boolean canKill = (oldY < creature.getY());
            checkPlayerCollision((Player)creature, canKill);
        }
        if (creature instanceof Grub) {
        	if (creature.getVelocityX() != 0f) {
        		
        		
        		if((creature.badsct > 0 && 
            			System.currentTimeMillis() - creature.creaturesct > 800) ||
            			(creature.badsct == 0 &&
            			((map.getPlayer().getVelocityX()==0 && System.currentTimeMillis() - creature.creaturesct > 2000) || 
            			(map.getPlayer().getVelocityX()!=0 && System.currentTimeMillis() - creature.creaturesct > 500)))){
        			CreatureFire creaturefire = (CreatureFire)resourceManager.getcreaturefireSprite().clone();
        			if (!creature.to_L) {
        				creaturefire.setX(creature.getX() + 10);
        				creaturefire.setY(creature.getY() + 10);
        				creaturefire.setVelocityX(0.5f);
        			}
        			else{
        				creaturefire.setX(creature.getX() - 10);
        				creaturefire.setY(creature.getY() + 10);
        				creaturefire.setVelocityX(-0.5f);
        			}
        			creature.creaturesct = System.currentTimeMillis();
        			creature.badsct = creature.badsct + 1;
        			return creaturefire;
        		}
        	}
        	else{
        		creature.creaturesct = System.currentTimeMillis();
        	}
        }
        return null;
    }

    public void checkshoot (Fire fire, Player player){
    	Sprite Spritecollision = getSpriteCollision(fire);
    	if (Spritecollision != null){
    		fire.setState(2);
    		if (Spritecollision instanceof Grub) {
    			soundManager.play(bugshoot);
    			Creature badcreature = (Creature)Spritecollision;
    			badcreature.setState(1);
    			player.health += 10;
    		}
    		if (Spritecollision instanceof CreatureFire) {
    			((Creature)Spritecollision).setState(Creature.STATE_DEAD);
    		}
    	}
    }
    

    /**
        Checks for Player collision with other Sprites. If
        canKill is true, collisions with Creatures will kill
        them.
    */
    public void checkPlayerCollision(Player player,
        boolean canKill)
    {
        if (!player.isAlive()) {
        	timepass = 0;
            return;
        }

        // check for player collision with other sprites
        Sprite collisionSprite = getSpriteCollision(player);
        if (collisionSprite instanceof PowerUp) {
            acquirePowerUp((PowerUp)collisionSprite, player);
        }
        else if (collisionSprite instanceof CreatureFire){
        	Creature badguy = (Creature)collisionSprite;
        	badguy.setState(Creature.STATE_DEAD);
        	player.health -= 5;
        	if (player.getHealth() == 0){
        		player.setState(Creature.STATE_DYING);
        	}
        }
        else if (collisionSprite instanceof Fire) {
        }
        
        else if (collisionSprite instanceof Fly) {
            Creature badguy = (Creature)collisionSprite;
            if (canKill) {
                // kill the badguy and make player bounce
                soundManager.play(boopSound);
                badguy.setState(Creature.STATE_DYING);
                player.setY(badguy.getY() - player.getHeight());
                player.jump(true);
            }
            else {
                // player dies!
                player.setState(Creature.STATE_DYING);
            }
        }
        else if (collisionSprite instanceof Grub) {
            Creature badguy = (Creature)collisionSprite;
            if (canKill) {
                // kill the badguy and make player bounce
                soundManager.play(boopSound);
                badguy.setState(Creature.STATE_DYING);
                player.setY(badguy.getY() - player.getHeight());
                player.jump(true);
            }
            else {
                // player dies!
                player.setState(Creature.STATE_DYING);
            }
        }
        else if (collisionSprite instanceof Star) {
        	player.invincible = true;
        	map.removeSprite(collisionSprite);
        }
    }


    /**
        Gives the player the speicifed power up and removes it
        from the map.
    */
    public void acquirePowerUp(PowerUp powerUp, Player player) {
        // remove it from the map
        

        if (powerUp instanceof PowerUp.Star) {
            // do something here, like give the player points
            soundManager.play(prizeSound);
            map.removeSprite(powerUp);
        }
        else if (powerUp instanceof PowerUp.Music) {
            // change the music
            soundManager.play(prizeSound);
            toggleDrumPlayback();
            map.removeSprite(powerUp);
        }
        else if (powerUp instanceof PowerUp.Goal) {
            // advance to next map
            soundManager.play(prizeSound,
                new EchoFilter(2000, .7f), false);
            map = resourceManager.loadNextMap();
            map.removeSprite(powerUp);
        }
        else if (powerUp instanceof PowerUp.Gas) {
        	
        }
        else if (powerUp instanceof PowerUp.Explode) {
        	map.removeSprite(powerUp);
        	player.health -= 10;
        }
        else if (powerUp instanceof PowerUp.Mushroom) {
        	map.removeSprite(powerUp);
        	player.health += 5;
        	soundManager.play(prizeSound);
        }
    }

}

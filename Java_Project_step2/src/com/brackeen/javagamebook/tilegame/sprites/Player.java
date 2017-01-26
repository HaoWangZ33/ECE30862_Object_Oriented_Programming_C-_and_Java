package com.brackeen.javagamebook.tilegame.sprites;
import com.brackeen.javagamebook.tilegame.GameManager;
import com.brackeen.javagamebook.test.GameCore;
import com.brackeen.javagamebook.graphics.Animation;


/**
    The Player.
*/
public class Player extends Creature {
	public int health = 20;
	private final int healthmx=40;
	public boolean invincible = false;

    private static final float JUMP_SPEED = -.95f;

    private boolean onGround;

    public Player(Animation left, Animation right,
        Animation deadLeft, Animation deadRight)
    {
        super(left, right, deadLeft, deadRight);
    }


    public void collideHorizontal() {
        setVelocityX(0);
    }


    public void collideVertical() {
        // check if collided with ground
        if (getVelocityY() > 0) {
            onGround = true;
        }
        setVelocityY(0);
    }


    public void setY(float y) {
        // check if falling
        if (Math.round(y) > Math.round(getY())) {
            onGround = false;
        }
        super.setY(y);
    }


    public void wakeUp() {
        // do nothing
    }


    /**
        Makes the player jump if the player is on the ground or
        if forceJump is true.
    */
    public void jump(boolean forceJump) {
        if (onGround || forceJump) {
            onGround = false;
            setVelocityY(JUMP_SPEED);
        }
    }


    public float getMaxSpeed() {
        return 0.5f;
    }
    
    public int getHealth(){
    	if(isAlive() == false){
    		return 0;//if dead health is equal to 0
    	}
    	int volocityPlayer = Math.round(Math.abs(getVelocityX()));
    	
    	if(onGround != true || volocityPlayer !=0){
    		if(GameCore.timepass >= 1000){
    			GameCore.timepass = 0;
    			health =health +1;
    		}
    	}//Not motionless
    	else if(onGround = true && volocityPlayer ==0){
    		if(GameCore.timepass>=1000){
    			GameCore.timepass=0;
    			health = health +5;
    		}
    	}//Motionless
    	if(health >= healthmx){
    		health = healthmx;
    	}else if(health < 0){
    		health = 0;
    	}
    	return health;
    }

}

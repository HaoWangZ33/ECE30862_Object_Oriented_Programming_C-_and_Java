#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<string>
#include<list>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "Attack.hpp"
#include "Container.hpp"
#include "Creature.hpp"
#include "Item.hpp"
#include "Room.hpp"
#include "Trigger.hpp"
using namespace std;
using namespace rapidxml;

void changeRoom(string input);
void showInventory();
void take(string input);
void execute_Input(string input);
string checktype(string target);
void attack(string target, string weapon);
void open(string input);
void exit();
void turnon(string input);
void put(string a, string b);
void read(string input);
void drop(string input);
void GameOver();
void Add(string input);
void Delete(string object);
void Update(string input);
void checkAction(string input);
bool Trigger_withoutcommand();
bool Trigger_withcommand(string input);
bool check_status_trigger(Trigger * t);
bool check_owner_trigger(Trigger * t);


vector<Room*> room_list;
vector<Item*> item_list;
vector<Container*> container_list;
vector<Creature*> creature_list;
vector<string> inventory;
bool GAMEOVER = false;
Room * location;

void execute_Input(string input){
    if(input == "n" || input == "s" || input == "w" || input == "e"){
        changeRoom(input);
        return;
    }
    if(input == "i"){
        showInventory();
        return;
    }
    if(input == "take"){
        cout<<"What do you want to take? ";
        getline(cin, input);
        take(input);
        return;
    }
    if(input.find("take") != string::npos){
        input.erase(0,5);
        take(input);
        return;
    }
    if(input == "open"){
    	cout<<"What do you want to open? ";
    	getline(cin, input);
    	if(input == "exit"){
    		exit();
    		return;
    	}
    	open(input);
    	return;
    }
    if(input.find("open") != string::npos){
    	input.erase(0,5);
    	if(input == "exit"){
    		exit();
    		return;
    	}
    	open(input);
    	return;
    }
    if(input == "read"){
		cout<<"What do you want to read? ";
		getline(cin, input);
		read(input);
		return;
	}
	if(input.find("read") != string::npos){
		input.erase(0,5);
		read(input);
		return;
	}
    if(input == "drop"){
        cout<<"What do you want to drop? ";
        getline(cin, input);
        drop(input);
        return;
    }
    if(input.find("drop") != string::npos){
        input.erase(0,5);
        drop(input);
        return;
    }
    if(input == "put"){
        cout << "What do you want to put? ";
        getline(cin, input);
        string item_put = input;
        cout << "Where do you want to put in? ";
        getline(cin, input);
        string where_put = input;
        put(item_put, where_put);
        return;
    }
    if(input.find("put") != string::npos){
        input.erase(0,4);
        int space_pos = input.find(" "); //it's also size of item string
        string item_put = input.substr(0, space_pos);
        input.erase(0, space_pos + 4);
        string where_put = input;
        put(item_put, where_put);
        return;
    }
    if(input == "turn on"){
        cout<<"What do you want to turn on? ";
        getline(cin, input);
        turnon(input);
        return;
    }
    if(input.find("turn on") != string::npos){
        input.erase(0,8);
        turnon(input);
        return;
    }
    if(input == "attack"){
        cout << "What do you want to attack? ";
        getline(cin, input);
        string target = input;
        cout << "What do you want to attack with? ";
        getline(cin, input);
        string weapon = input;
        attack(target, weapon);
        return;
    }
    if(input.find("attack") != string::npos){
        input.erase(0,7);
        int space_pos = input.find(" "); //it's also size of item string
        string target = input.substr(0, space_pos);
        input.erase(0, space_pos + 6);
        string weapon = input;
        attack(target, weapon);
        return;
    }
  cout << "Error" << endl;
}

string checktype(string target){
    int count;
    for(count = 0; count < room_list.size(); count++){
        if(room_list[count] -> name == target){
            return "Room";
        }
    }
    for(count = 0; count < item_list.size(); count++){
        if(item_list[count] -> name == target){
            return "Item";
        }
    }
    for(count = 0; count < container_list.size(); count++){
        if(container_list[count] -> name == target){
            return "Container";
        }
    }
    for(count = 0; count < creature_list.size(); count++){
        if(creature_list[count] -> name == target){
            return "Creature";
        }
    }
    return NULL;
}

void attack(string target, string weapon){
    int count, count2;
    for(count = 0; count < location -> creature.size(); count++){
        if(location -> creature[count] == target){ break; }
    }
    if(count >= location -> creature.size()){
        cout<<"There is no such creature called "<<target<<" here."<<endl;
        return;
    }
    for(count = 0; count < inventory.size(); count++){
        if(inventory[count] == weapon){ break; }
    }
    if(count >= inventory.size()){
        cout<<"There is no such thing called "<<weapon<<" in your inventory."<<endl;
        return;
    }
    cout<<"You assault the "<<target<<" with the "<<weapon<<endl;
    for(count2 = 0; count2 < creature_list[count]->vulnerability.size();count2++){
            if(creature_list[count]->vulnerability[count2] == weapon){ break;}
        }
    for(count = 0; count < creature_list.size(); count++){
        if(creature_list[count]->name == target){ break;}
    }
    if(count2 == creature_list[count]->vulnerability.size()){
        cout<<"It seems "<<weapon<<" is useless to "<<target<<endl;
        return;
    }
    if(creature_list[count]->attack == NULL){
        return;
    }
    if(creature_list[count]->attack->has_condition){
    	string status = creature_list[count]->attack->condition.status;
        string object = creature_list[count]->attack->condition.object;
        bool match = false;
        bool find = false;
        for(count2 = 0; count2 < item_list.size(); count2++){
            if(item_list[count2]->name == object){
                find = true;
                match = item_list[count2]->status == status;
                break;
            }
        }
        if(!find){
                    for(count2 = 0; count2 < creature_list.size(); count2++){
                        if(creature_list[count2]->name == object){
                            find = true;
                            match = creature_list[count2]->status == status;
                            break;
                        }
                    }
                }
        if(!find){
            for(count2 = 0; count2 < room_list.size(); count2++){
                if(room_list[count2]->name == object){
                    find = true;
                    match = room_list[count2]->status == status;
                    break;
                }
            }
        }
        if(!find){
            for(count2 = 0; count2 < container_list.size(); count2++){
                if(container_list[count2]->name == object){
                    find = true;
                    match = container_list[count2]->status == status;
                    break;
                }
            }
        }
        if(!match){
            cout<<"You need make sure "<<object<<" is "<<status<<endl;
            return;
        }
    }
    if(creature_list[count]->attack->has_print){
        cout<<creature_list[count]->attack->print<<endl;
    }
    if(creature_list[count]->attack->has_move){
        for(count2 = 0; count2<creature_list[count]->attack->move.size();count2++){
            checkAction(creature_list[count]->attack->move[count2]);
        }
    }
}

void open(string input){
    int count, count2, count3;
    for(count = 0; count < location -> container.size(); count++){
        if(location -> container[count] == input){
            break;
        }
    }
    if(count == location -> container.size()){
        cout << "There is no such thing here to open." << endl;
        return;
    }else{
		for(count2 = 0; count2 < container_list.size(); count2++){
			if(container_list[count2] -> name == input){
				if(container_list[count2] -> status == "locked"){
					cout << container_list[count2] -> name << " is locked." << endl;
					return;
				}
				else if((container_list[count2] -> item).size() == 0){
					cout <<container_list[count2] -> name << " is empty." << endl;
					return;
				}else{
					container_list[count2] -> status = "unlocked";
					cout << container_list[count2] -> name << " contains ";
					for(count3 = 0; count3 < (container_list[count2] -> item).size() - 1; count3++){
						cout << container_list[count2] -> item[count3] << ", ";
					}
					cout << container_list[count2] -> item[count3] << endl;
					return;
				}
			}
		}
    }
}

void exit(){
    if(location -> type == "exit"){
        GameOver();
        return;
    }
    cout << "There is no exit in this room" << endl;
}

void turnon(string input){
	int count, count2;
    for(count = 0; count < inventory.size(); count++){
        if(inventory[count] == input){
            for(count2 = 0; count2 < item_list.size(); count2++){
                if(item_list[count2] -> name == input){
                    if(item_list[count2] -> can_turnon == false){
                        cout<<"Item is not able to be turned on." << endl;
                    }else{
                        cout<<"You activate the "<< item_list[count2]->name<<"."<<endl;
                        cout << item_list[count2] -> turnon.print << endl;
                        checkAction(item_list[count2] -> turnon.action);
                    }
                }
            }
            return;
        }
    }
    cout << "There is no such thing in your inventory to turn on." << endl;
}

void put(string a, string b){
    int count, count2, count3;
    for(count = 0; count < inventory.size(); count++){
        if(inventory[count] == a){
            if(location -> name == b){
                drop(a);
                return;
            }
            for(count2 = 0; count2 < location -> container.size(); count2++){
                if(location -> container[count2] == b){
                    break;
                }
            }
            if(count2 == location -> container.size()){
                cout<<"There is no such thing called "<<b<<" in this room."<<endl;
                return;
            }
            for(count2 = 0; count2 < container_list.size(); count2++){
                if(container_list[count2] -> name == b){
                    if(container_list[count2]->accept.size() == 0){
                        container_list[count2] -> item.push_back(a);
                        inventory.erase(inventory.begin() + count);
                        cout << a << " has been put to "<<b<<endl;
                        if(container_list[count2]->status == ""){
                            container_list[count2]->status = "unlocked";
                        }
                        return;
                    }
                    for(count3 = 0;count3 < container_list[count2]->accept.size();count3++){
                        if(container_list[count2] -> accept[count3] == a){
                            container_list[count2] -> item.push_back(a);
                            inventory.erase(inventory.begin() + count);
                            cout << a << " has been put to "<<b<<endl;
                            if(container_list[count2]->status == ""){
                                container_list[count2]->status = "unlocked";
                            }
                            return;
                        }
                    }
                    cout << b << " doesn't accept " << a << endl;
                    return;
                }
            }
            return;
        }
    }
    cout << "There is no such thing in your inventory. "<<endl;
}

void read(string input){
	int count, count2;
    for(count = 0; count < inventory.size(); count++){
        if(inventory[count] == input){
            for(count2 = 0; count2 < item_list.size(); count2++){
                if(item_list[count2] -> name == input){
                    if(item_list[count2] -> writing.size() == 0){
                        cout<<"There's nothing to read." << endl;
                    }else{
                        cout << item_list[count2] -> writing << endl;
                    }
                }
            }
            return;
        }
    }
    cout << "There is no such thing in your inventory to read." << endl;
}

void drop(string input){
	int count;
    for(count = 0; count < inventory.size(); count++){
        if(inventory[count] == input){
            (location -> item).push_back(input);
            inventory.erase(inventory.begin() + count);
            cout << input << " has been dropped." << endl;
            return;
        }
    }
    cout << "There is no such thing to drop." << endl;
}

void take(string input){
    int count1,count2,count3;
    for(count1 = 0; count1 < (location -> container).size(); count1++){
            string container_name = location->container[count1];
            for(count2 = 0; count2 < container_list.size(); count2++){
                if(container_list[count2]->name == container_name &&
                   container_list[count2]->status == "unlocked"){
                    for(count3 = 0; count3 < container_list[count2]->item.size();count3++){
                        if(container_list[count2]->item[count3] == input){
                            (location -> item).push_back(container_list[count2] -> item[count3]);
                            container_list[count2] -> item.erase(container_list[count2]->item.begin()+count3);
                            take(input);
                            return;
                        }
                    }
                }
            }
        }
	for(count1 = 0; count1 < (location -> item).size(); count1++){
		if(location -> item[count1] == input){
			inventory.push_back(input);
			(location -> item).erase((location -> item).begin() + count1);
            cout << input << " has been added to inventory." << endl;
			return;
		}
	}
	cout << "There is no such thing to take." << endl;
}

void showInventory(){
	int size = inventory.size();
	if(size == 0){
		cout << "Inventory: Empty" << endl;
		return;
	}
	cout << "Inventory: ";
	int count;
	for(count = 0; count < size - 1; count++){
		cout << inventory[count] << ",";
	}
	cout << inventory[count] << endl;
}

void changeRoom(string input){
    int size = (location -> border).size();
    int count, count2;
    for(count = 0; count < size; count++){
        if(location -> border[count] -> direction == input){
            string newroom = location -> border[count] -> name;
            int size2 = room_list.size();
            for(count2 = 0; count2 < size2; count2++){
                if(room_list[count2] -> name == newroom){
                    location = room_list[count2];
                }
            }
            break;
    }
  }
    if(count == size){
        cout << "You can't go that way" << endl;
    }else{
        cout << location -> description << endl;
    }
}

void GameOver(){
    GAMEOVER = true;
    cout << "Victory!" << endl;
}

void Add(string input){
    string object, loc;
    int to_position = input.find(" to ");
    object = input.substr(0, to_position);
    loc = input.substr(to_position + 4);
    int count;
    string type = checktype(object);
    if(type == "Item"){
        for(count = 0; count < room_list.size(); count++){
            if(room_list[count] -> name == loc){
                room_list[count] -> item.push_back(object);
                return;
            }
        }
        for(count = 0; count < container_list.size(); count++){
            if(container_list[count] -> name == loc){
                container_list[count] -> item.push_back(object);
                return;
            }
        }
    }
    if(type == "Container"){
            for(count = 0; count < room_list.size(); count++){
                if(room_list[count]->name == loc){
                    room_list[count]->container.push_back(object);
                    return;
                }
            }
        }
    if(type == "Creature"){
        for(count = 0; count < room_list.size(); count++){
            if(room_list[count]->name == loc){
                room_list[count]->creature.push_back(object);
                return;
            }
        }
    }
}



void Delete(string object){
    int count1, count2, count3;
    for(count1 = 0; count1 < room_list.size(); count1++){
        if(room_list[count1]->name == object){
            for(count2 = 0; count2 < location -> border.size(); count2++){
                if(location -> border[count2] -> name == object){
                    location->border.erase(location->border.begin()+count2);
                    return;
                }
            }
        }
    }
    for(count1 = 0; count1 < container_list.size(); count1++){
        if(container_list[count1]->name == object){
            for(count2 = 0; count2 < room_list.size(); count2++){
                for(count3 = 0; count3 < room_list[count2] -> container.size(); count3++){
                    if(room_list[count2] -> container[count3] == object){
                        room_list[count2] -> container.erase(room_list[count2]->container.begin()+count3);
                        return;
                    }
                }
            }
        }
    }
    for(count1 = 0; count1 < item_list.size(); count1++){
        if(item_list[count1]->name == object){
            for(count2 = 0; count2 < room_list.size(); count2++){
                for(count3 = 0; count3 < room_list[count2] -> item.size(); count3++){
                    if(room_list[count2] -> item[count3] == object){
                        room_list[count2] -> item.erase(room_list[count2]->item.begin()+count3);
                        return;
                    }
                }
            }
        }
    }
    for(count1 = 0; count1 < creature_list.size(); count1++){
        if(creature_list[count1]->name == object){
            for(count2 = 0; count2 < room_list.size(); count2++){
                for(count3 = 0; count3 < room_list[count2] -> creature.size(); count3++){
                    if(room_list[count2] -> creature[count3] == object){
                        room_list[count2] -> creature.erase(room_list[count2]->creature.begin()+count3);
                        return;
                    }
                }
            }
        }
    }
}

void Update(string input){
    string object, next_status;
    int to_position = input.find(" to ");
    object = input.substr(0, to_position);
    next_status = input.substr(to_position + 4);
    int count;
    for(count = 0; count < room_list.size(); count++){
        if(room_list[count]->name == object){
            room_list[count]->status = next_status;
            return;
        }
    }
    for(count = 0; count < item_list.size(); count++){
            if(item_list[count]->name == object){
                item_list[count]->status = next_status;
                return;
            }
        }
    for(count = 0; count < container_list.size(); count++){
        if(container_list[count]->name == object){
            container_list[count]->status = next_status;
            return;
        }
    }
    for(count = 0; count < creature_list.size(); count++){
        if(creature_list[count]->name == object){
            creature_list[count]->status = next_status;
            return;
        }
    }
}

void checkAction(string input){
    if(input.find("Add") != string::npos){
        Add(input.erase(0,4));
        return;
    }
    if(input.find("Delete") != string::npos){
        Delete(input.erase(0,7));
        return;
    }
    if(input.find("Update") != string::npos){
        Update(input.erase(0,7));
        return;
    }
    if(input == "Game Over"){
        GameOver();
        return;
    }
    execute_Input(input);
}

bool Trigger_withcommand(string input){
    int count1, count2, count3;
    bool room_trigger = false;
    bool item_trigger = false;
    bool container_trigger = false;
    bool creature_trigger = false;
    Trigger *t;
    if(location -> trigger.size() != 0){
        for(count2 = 0; count2 < location->trigger.size(); count2++){
            t = location->trigger[count2];
            if(t -> has_command && input == t -> command){
                if(t->type == "permanent" || (t->type == "single" && t->instances == 0)){
                    if(t->condition == 3){
                    	room_trigger = check_owner_trigger(t);
                    }
                    else if(t->condition == 2){
                    	room_trigger = check_status_trigger(t);
                    }
                    if(room_trigger == true){
                    	t->instances++;
                    }
                }
            }
        }
    }
    for(count1 = 0; count1 < inventory.size(); count1++){
        string target = inventory[count1];
        for(count2 = 0; count2 < item_list.size(); count2++){
            if(item_list[count2] -> name == target){
                for(count3 = 0; count3 < item_list[count2] -> trigger.size(); count3++){
                    t = item_list[count2] -> trigger[count3];
                    if(t -> has_command && input == t -> command){
                        if(t->type == "permanent" || (t->type == "single" && t->instances == 0)){
                            if(t->condition == 3){
                            	item_trigger = check_owner_trigger(t);
                            }
                            else if(t->condition == 2){
                            	item_trigger = check_status_trigger(t);
                            }
                            if(item_trigger == true){
                            	t->instances++;
                            }
                        }
                    }
                }
            }
        }
    }
    for(count1 = 0; count1 < location -> container.size(); count1++){
        string target = location -> container[count1];
        for(count2 = 0; count2 < container_list.size(); count2++){
            if(container_list[count2] -> name == target){
                for(count3 = 0; count3 < container_list[count2] -> trigger.size(); count3++){
                    t = container_list[count2] -> trigger[count3];
                    if(t -> has_command && input == t -> command){
                        if(t->type == "permanent" || (t->type == "single" && t->instances == 0)){
                            if(t->condition == 3){
                            	container_trigger = check_owner_trigger(t);
                            }
                            else if(t->condition == 2){
                            	container_trigger = check_status_trigger(t);
                            }
                            if(container_trigger == true){
                            	t->instances++;
                            }
                        }
                    }
                }
            }
        }
    }
    for(count1 = 0; count1 < location -> creature.size(); count1++){
        string target = location -> creature[count1];
        for(count2 = 0; count2 < creature_list.size(); count2++){
            if(creature_list[count2] -> name == target){
                for(count3 = 0; count3 < creature_list[count2] -> trigger.size(); count3++){
                    t = creature_list[count2] -> trigger[count3];
                    if(t -> has_command && input == t -> command){
                        if(t->type == "permanent" || (t->type == "single" && t->instances == 0)){
                            if(t->condition == 3){
                            	creature_trigger = check_owner_trigger(t);
                            }
                            else if(t->condition == 2){
                            	creature_trigger = check_status_trigger(t);
                            }
                            if(creature_trigger == true){
                            	t->instances++;
                            }
                        }
                    }
                }
            }
        }
    }
    return (room_trigger||item_trigger||container_trigger||creature_trigger);
}

bool Trigger_withoutcommand(){
	int count1, count2, count3;
	bool room_trigger = false;
	bool item_trigger = false;
	bool container_trigger = false;
	bool creature_trigger = false;
	Trigger *t;
    if(location -> trigger.size() != 0){
        for(count2 = 0; count2 < location->trigger.size(); count2++){
            t = location->trigger[count2];
            if(!t->has_command){
                if(t->type == "permanent" || (t->type == "single" && t->instances == 0)){
                    if(t->condition == 3){
                    	room_trigger = check_owner_trigger(t);
                    }
                    else if(t->condition == 2){
                    	room_trigger = check_status_trigger(t);
                    }
                    if(room_trigger == true){
                    	t->instances++;
                    }
                }
            }
        }
    }

    for(count1 = 0; count1 < location -> item.size(); count1++){
        string target = location -> item[count1];
        for(count2 = 0; count2 < item_list.size(); count2++){
            if(item_list[count2] -> name == target){
                for(count3 = 0; count3 < item_list[count2] -> trigger.size(); count3++){
                    t = item_list[count2] -> trigger[count3];
                    if(!t->has_command){
                        if(t->type == "permanent" || (t->type == "single" && t->instances == 0)){
                            if(t->condition == 3){
                            	item_trigger = check_owner_trigger(t);
                            }
                            else if(t->condition == 2){
                            	item_trigger = check_status_trigger(t);
                            }
                            if(item_trigger == true){
                            	t->instances++;
                            }
                        }
                    }
                }
            }
        }
    }

    for(count1 = 0; count1 < location -> container.size(); count1++){
        string target = location -> container[count1];
        for(count2 = 0; count2 < container_list.size(); count2++){
            if(container_list[count2] -> name == target){
                for(count3 = 0; count3 < container_list[count2] -> trigger.size(); count3++){
                    t = container_list[count2] -> trigger[count3];
                    if(!t->has_command){
                        if(t->type == "permanent" || (t->type == "single" && t->instances == 0)){
                            if(t->condition == 3){
                            	container_trigger = check_owner_trigger(t);
                            }
                            else if(t->condition == 2){
                            	container_trigger = check_status_trigger(t);
                            }
                            if(container_trigger == true){
                            	t->instances++;
                            }
                        }
                    }
                }
            }
        }
    }

    for(count1 = 0; count1 < location -> creature.size(); count1++){
        string target = location -> creature[count1];
        for(count2 = 0; count2 < creature_list.size(); count2++){
            if(creature_list[count2] -> name == target){
                for(count3 = 0; count3 < creature_list[count2] -> trigger.size(); count3++){
                    t = creature_list[count2] -> trigger[count3];
                    if(!t->has_command){
                        if(t->type == "permanent" || (t->type == "single" && t->instances == 0)){
                            if(t->condition == 3){
                            	creature_trigger = check_owner_trigger(t);
                            }
                            else if(t->condition == 2){
                            	creature_trigger = check_status_trigger(t);
                            }
                            if(creature_trigger == true){
                            	t->instances++;
                            }
                        }
                    }
                }
            }
        }
    }
    return (room_trigger||item_trigger||container_trigger||creature_trigger);
}

bool check_status_trigger(Trigger * t){
    string object = t -> status.object;
    string status = t -> status.status;
    int count1, count2;
    string object_type = checktype(object);
    if(object_type == "Room"){
        for(count1 = 0; count1 < room_list.size(); count1++){
            if(room_list[count1] -> name == object){
                if(room_list[count1] -> status == status){
                    if(t->has_print){
                    	cout<<t->print<<endl;
                    }
                    if(t->has_action){
                        for(count2 = 0; count2 < t->action.size();count2++){
                            checkAction(t->action[count2]);
                        }
                    }
                    return true;
                }
            }
        }
    }
    else if(object_type == "Item"){
        for(count1 = 0; count1 < item_list.size(); count1++){
            if(item_list[count1] -> name == object){
                if(item_list[count1] -> status == status){
                    if(t->has_print){
                    	cout<<t->print<<endl;
                    }
                    if(t->has_action){
                        for(count2 = 0; count2 < t->action.size();count2++){
                            checkAction(t->action[count2]);
                        }
                    }
                    return true;
                }
            }
        }
    }
    else if(object_type == "Container"){
        for(count1 = 0; count1 < container_list.size(); count1++){
            if(container_list[count1] -> name == object){
                if(container_list[count1] -> status == status){
                    if(t->has_print){
                    	cout<<t->print<<endl;
                    }
                    if(t->has_action){
                        for(count2 = 0; count2 < t->action.size();count2++){
                            checkAction(t->action[count2]);
                        }
                    }
                    return true;
                }
            }
        }
    }
    else if(object_type == "Creature"){
        for(count1 = 0; count1 < creature_list.size(); count1++){
            if(creature_list[count1] -> name == object){
                if(creature_list[count1] -> status == status){
                    if(t->has_print){
                    	cout<<t->print<<endl;
                    }
                    if(t->has_action){
                        for(count2 = 0; count2 < t->action.size();count2++){
                            checkAction(t->action[count2]);
                        }
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

bool check_owner_trigger(Trigger * t){
	int count1, count2, count3;
    string owner = t -> owner.owner;
    string has = t -> owner.has;
    string object = t -> owner.object;
    if(owner == "inventory"){
        for(count1 = 0; count1 < inventory.size(); count1++){
            if(inventory[count1] == object){
                if(has == "yes"){
                    if(t->has_print){
                    	cout<<t->print<<endl;
                    }
                    if(t->has_action){
                        for(count3 = 0; count3 < t->action.size();count3++){
                            checkAction(t->action[count3]);
                        }
                    }
                    return true;
                }else{
                	return false;
                }
            }
        }
        if(count1 == inventory.size()){
            if(has == "no"){
                if(t->has_print){
                	cout<<t->print<<endl;
                }
                if(t->has_action){
                    for(count3 = 0; count3 < t->action.size();count3++){
                        checkAction(t->action[count3]);
                    }
                }
                return true;
            }else{
            	return false;
            }
        }
    }
    string type_owner = checktype(owner);
    string type_object = checktype(object);
    if(type_owner == "Room"){
        for(count1 = 0; count1 < room_list.size(); count1++){
            if(room_list[count1] -> name == owner){
                if(type_object == "Item"){
                    for(count2 = 0; count2 < room_list[count1] -> item.size(); count2++){
                        if(room_list[count1]->item[count2] == object){
                            if(has == "yes"){
                                if(t->has_print){
                                	cout<<t->print<<endl;
                                }
                                if(t->has_action){
                                    for(count3 = 0; count3 < t->action.size();count3++){
                                        checkAction(t->action[count3]);
                                    }
                                }
                                return true;
                            }else{
                            	return false;
                            }
                        }
                    }
                    if(count2 == room_list[count1]->item.size()){
                        if(has == "no"){
                            if(t->has_print){
                            	cout<<t->print<<endl;
                            }
                            if(t->has_action){
                                for(count3 = 0; count3 < t->action.size();count3++){
                                    checkAction(t->action[count3]);
                                }
                            }
                            return true;
                        }else{
                        	return false;
                        }
                    }
                }
                if(type_object == "Container"){
                    for(count2 = 0; count2 < room_list[count1] -> container.size(); count2++){
                        if(room_list[count1]->container[count2]== object ){
                            if(has == "yes"){
                                if(t->has_print){
                                	cout<<t->print<<endl;
                                }
                                if(t->has_action){
                                    for(count3 = 0; count3 < t->action.size();count3++){
                                        checkAction(t->action[count3]);
                                    }
                                }
                                return true;
                            }else{
                            	return false;
                            }
                        }
                    }
                    if(count2 == room_list[count1]->container.size()){
                        if(has == "no"){
                            if(t->has_print){
                            	cout<<t->print<<endl;
                            }
                            if(t->has_action){
                                for(count3 = 0; count3 < t->action.size();count3++){
                                    checkAction(t->action[count3]);
                                }
                            }
                            return true;
                        }else{
                        	return false;
                        }
                    }
                }
                if(type_object == "Creature"){
                    for(count2 = 0; count2 < room_list[count1] -> creature.size(); count2++){
                        if(room_list[count1]->creature[count2]== object){
                            if(has == "yes"){
                                if(t->has_print){
                                	cout<<t->print<<endl;
                                }
                                if(t->has_action){
                                    for(count3 = 0; count3 < t->action.size();count3++){
                                        checkAction(t->action[count3]);
                                    }
                                }
                                return true;
                            }else{
                            	return false;
                            }
                        }
                    }
                    if(count2 == room_list[count1]->creature.size()){
                        if(has == "no"){
                            if(t->has_print){
                            	cout<<t->print<<endl;
                            }
                            if(t->has_action){
                                for(count3 = 0; count3 < t->action.size();count3++){
                                    checkAction(t->action[count3]);
                                }
                            }
                            return true;
                        }else{
                        	return false;
                        }
                    }
                }
            }
        }
    }
    if(type_owner == "Container"){
        for(count1 = 0; count1 < container_list.size(); count1++){
            if(container_list[count1]->name == owner){
                for(count2 = 0; count2 < container_list[count1] -> item.size(); count2++){
                    if(container_list[count1]->item[count2]== object){
                        if(has == "yes"){
                            if(t->has_print){
                            	cout<<t->print<<endl;
                            }
                            if(t->has_action){
                                for(count3 = 0; count3 < t->action.size();count3++){
                                    checkAction(t->action[count3]);
                                }
                            }
                            return true;
                        }else{
                        	return false;
                        }
                    }
                }
                if(count2 == container_list[count1]->item.size()){
                    if(has == "no"){
                        if(t->has_print){
                        	cout<<t->print<<endl;
                        }
                        if(t->has_action){
                            for(count3 = 0; count3 < t->action.size();count3++){
                                checkAction(t->action[count3]);
                            }
                        }
                        return true;
                    }else{
                    	return false;
                    }
                }
            }
        }
    }
    return false;
}


int main(int argc, char *argv[]){
    if( argc != 2 ){
        cout << "Error, please enter a xml file name as argument." <<endl;
        return 1;
    }

    file <> inputFile(argv[1]);
    xml_document <> infile;
    infile.parse<0>(inputFile.data());
    
    xml_node <> *node = infile.first_node();
    xml_node <> *top_node;

    vector<xml_node<>*> room_node;
    vector<xml_node<>*> item_node;
    vector<xml_node<>*> container_node;
    vector<xml_node<>*> creature_node;

    for( top_node = node -> first_node(); top_node; top_node = top_node -> next_sibling() ){
    	if( string(top_node -> name()) == string("room") ){
    		room_node.push_back(top_node);
    	}
    	if( string(top_node -> name()) == string("item") ){
			item_node.push_back(top_node);
		}
    	if( string(top_node -> name()) == string("container") ){
    		container_node.push_back(top_node);
    	}
    	if( string(top_node -> name()) == string("creature") ){
    		creature_node.push_back(top_node);
    	}
    }

    int size, count;
//add rooms to room list
    Room *room;
    size = room_node.size();
    count = 0;
    while(count < size){
      room = new Room(room_node[count]);
      room_list.push_back(room);
      count++;
    }

//add items to item list
    Item * item;
	size = item_node.size();
	count = 0;
	while(count < size){
		item = new Item(item_node[count]);
		item_list.push_back(item);
		count++;
	}

//add container to container list
	Container * container;
    size = container_node.size();
    count = 0;
    while(count < size){
    	container = new Container(container_node[count]);
    	container_list.push_back(container);
    	count++;
    }

//add creature to creature list
    Creature * creature;
    size = creature_node.size();
    count = 0;
    while(count < size){
      creature = new Creature(creature_node[count]);
      creature_list.push_back(creature);
      count++;
    }

    bool override = false;
    string u_input;
    location = room_list[0];
    cout<< location -> description <<endl;
    while(true){
        override = Trigger_withoutcommand();
        if(GAMEOVER){break;}
        if(override){continue;}
        getline(cin, u_input);
        if(string(u_input) == string("q")){break;}
        override = Trigger_withcommand(u_input);
        if(GAMEOVER){break;}
        if(override){continue;}
        execute_Input(u_input);
        if(GAMEOVER){break;}
    }
    return 0;
}

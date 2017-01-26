#ifndef _ROOM_H
#define _ROOM_H

#include<cstdlib>
#include<vector>
#include<string>
#include<list>
#include<iostream>
#include<sstream>
#include<fstream>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "Trigger.hpp"
using namespace std;
using namespace rapidxml;

typedef struct _border{
  string direction;
  string name;
}Border;

class Room{
public:
	string name;
	string status;
	string type;
	string description;
	vector<Border*> border;
	vector<string> container;
	vector<string> item;
	vector<string> creature;
    vector<Trigger*> trigger;
    
	Room(xml_node<>* node){
		setup(node);
	}
	virtual ~Room(){};
private:
	void setup(xml_node<>* room){
		string buffer;
		for(xml_node<>*temp = room -> first_node(); temp; temp = temp -> next_sibling()){
			if(string(temp->name()) == "name"){
				name = temp -> value();
			}
			if(string(temp->name()) == "status"){
				status = temp->value();
			}
			if(string(temp->name()) == "type"){
				type = temp -> value();
			}
			if(string(temp->name()) == "description"){
				description = temp -> value();
			}
			if(string(temp->name()) == "border"){
				Border * new_border = new Border();
				string border_name, direction;
				for(xml_node<>*temp2 = temp -> first_node(); temp2; temp2 = (temp2 -> next_sibling())){
					if(string(temp2->name()) == "name"){
						border_name = temp2 -> value();
					}
					if(string(temp2->name()) == "direction"){
						if(string(temp2->value()) == "north"){direction = "n";}
						if(string(temp2->value()) == "south"){direction = "s";}
						if(string(temp2->value()) == "west"){direction = "w";}
						if(string(temp2->value()) == "east"){direction = "e";}
					}
				}
				new_border -> direction = direction;
				new_border -> name = border_name;
				border.push_back(new_border);
			}
			if(string(temp->name()) == "container"){
				buffer = temp -> value();
				container.push_back(buffer);
			}
			if(string(temp->name()) == "item"){
				buffer = temp -> value();
				item.push_back(buffer);
			}
			if(string(temp->name()) == "creature"){
				buffer = temp -> value();
				creature.push_back(buffer);
			}
			if(string(temp->name()) == "trigger"){
                Trigger * t = new Trigger(temp);
                trigger.push_back(t);
			}
       }
	}
};
#endif

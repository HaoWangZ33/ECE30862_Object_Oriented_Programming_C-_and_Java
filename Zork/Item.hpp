#ifndef _ITEM_H
#define _ITEM_H

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

typedef struct _turnon{
    string print;
    string action;
}Turnon;

class Item{
public:
	string name;
    string status;
	string description;
	string writing;
	bool can_turnon;
    Turnon turnon;
    vector<Trigger*> trigger;
    
	Item(xml_node<>* node){
		setup(node);
	}
	virtual ~Item(){};
private:
	void setup(xml_node<>* node){
		string print_value, action_value;
        can_turnon = false;
		for(xml_node<>*temp = node -> first_node(); temp; temp = temp -> next_sibling()){
			if(string(temp->name()) == "name"){
				name = temp -> value();
			}
			if(string(temp->name()) == "status"){
				status = temp -> value();
			}
			if(string(temp->name()) == "description"){
				description = temp -> value();
			}
			if(string(temp->name()) == "writing"){
				writing = temp -> value();
			}
            if(string(temp->name()) == "turnon"){
                can_turnon = true;
                for(xml_node<>*temp2 = temp -> first_node(); temp2; temp2 = temp2 -> next_sibling()){
                    if(string(temp2->name()) == "print"){
                        print_value = temp2 -> value();
                    }
                    if(string(temp2->name()) == "action"){
                        action_value = temp2->value();
                    }
                }
                turnon.print = print_value;
                turnon.action = action_value;
            }
            if(string(temp->name()) == "trigger"){
                Trigger * t = new Trigger(temp);
                trigger.push_back(t);
            }
		}
	}
protected:
};

#endif

#ifndef _TRIGGER_H
#define _TRIGGER_H

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
using namespace std;
using namespace rapidxml;

typedef struct _owner{
    string object;
    string has;
    string owner;
}Owner;

typedef struct _status{
    string object;
    string status;
}Status;

class Trigger{
public:
    string type;
    string print;
    vector<string> action;

    int condition;//2 = status   3 = owner
    string command;
    Owner owner;
    Status status;

    bool has_print;
    bool has_action;
    bool has_command;
    int instances;
	Trigger(xml_node<>* node){
        setup(node);
	}
	virtual ~Trigger(){};
private:
    void setup(xml_node<>* node){
    	has_print = false;
    	has_action = false;
        has_command = false;
        string buffer;
        type = "single";
        instances = 0;
        for(xml_node<>* temp = node -> first_node(); temp; temp = temp -> next_sibling()){
            if(string(temp->name()) == "type"){
                type = temp -> value();
            }
            else if(string(temp->name()) == "print"){
                has_print = true;
                print = temp -> value();
            }
            else if(string(temp->name()) == "action"){
				has_action = true;
				buffer = temp -> value();
				action.push_back(buffer);
			}
            else if(string(temp->name()) == "condition"){
				condition = count_condi(temp);
				if(condition == 2){
					Status_condition(temp);
				}else if(condition == 3){
					Owner_condition(temp);
				}
			}
            else if(string(temp->name()) == "command"){
                has_command = true;
                command = temp -> value();
            }
        }
    }
    
    int count_condi(xml_node<>* node){
        int count = 0;
        for(xml_node<>* temp = node -> first_node(); temp; temp = temp -> next_sibling()){
            count++;
        }
        return count;
    }
    void Status_condition(xml_node<>* node){
        for(xml_node<>* temp = node -> first_node(); temp; temp = temp -> next_sibling()){
            if(string(temp->name()) == "object"){
                status.object = temp -> value();
            }
            else if(string(temp->name()) == "status"){
                status.status = temp -> value();
            }
        }
    }
    void Owner_condition(xml_node<>* node){
        for(xml_node<>* temp = node -> first_node(); temp; temp = temp -> next_sibling()){
            if(string(temp->name()) == "object"){
                owner.object = temp -> value();
            }
            else if(string(temp->name()) == "has"){
                owner.has = temp -> value();
            }
            else if(string(temp->name()) == "owner"){
                owner.owner = temp -> value();
            }
        }
    }
};

#endif

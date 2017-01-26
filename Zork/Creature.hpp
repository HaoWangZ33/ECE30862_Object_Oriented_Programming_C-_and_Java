#ifndef _CREATURE_H
#define _CREATURE_H

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
#include "Attack.hpp"
using namespace std;
using namespace rapidxml;

class Creature{
public:
    string name;
    string status;
    string description;
    vector<string> vulnerability;
    Attack *attack;
    vector<Trigger*> trigger;
    
	Creature(xml_node<>* node){
        setup(node);
    }
	virtual ~Creature(){};
private:
    void setup(xml_node<>* node){
    	xml_node<>* temp;
    	string string_temp;
        for(temp = node -> first_node(); temp; temp = temp -> next_sibling()){
            if(string(temp->name()) == "name"){
                name = temp -> value();
            }
            if(string(temp->name()) == "status"){
                status = temp -> value();
            }
            if(string(temp->name()) == "description"){
                description = temp -> value();
            }
            if(string(temp->name()) == "vulnerability"){
                string_temp = temp -> value();
                vulnerability.push_back(string_temp);
            }
            if(string(temp->name()) == "attack"){
                attack = new Attack(temp);
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

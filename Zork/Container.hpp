#ifndef _CONTAINER_H
#define _CONTAINER_H

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

class Container{
public:
	string name;
    string status;
    string description;
    vector<string> accept;
    vector<string> item;
    vector<Trigger*> trigger;
    
	Container(xml_node<>* node){
		setup(node);
	}
	virtual ~Container(){};

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
            if(string(temp->name()) == "accept"){
            	string_temp = temp -> value();
            	accept.push_back(string_temp);
            }
            if(string(temp->name()) == "item"){
                string_temp = temp -> value();
                item.push_back(string_temp);
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

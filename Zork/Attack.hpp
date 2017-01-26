#ifndef _ATTACK_H
#define _ATTACK_H

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

typedef struct _condition{
    string object;
    string status;
}Condition;

class Attack{
public:
    Condition condition;
    string print;
    vector<string> move;
    bool has_condition;
    bool has_print;
    bool has_move;

	Attack(xml_node<>* node){
        setup(node);
	}
	virtual ~Attack(){};

private:
    void setup(xml_node<>* node){
        has_condition = false;
        has_print = false;
        has_move = false;
        for(xml_node<>*temp = node -> first_node(); temp; temp = temp -> next_sibling()){
            if(string(temp->name()) == "condition"){
                has_condition = true;
                for(xml_node<>*temp2 = temp -> first_node(); temp2; temp2 = temp2 -> next_sibling()){
                    if(string(temp2->name()) == "object"){
                        condition.object = temp2->value();
                    }
                    if(string(temp2->name()) == "status"){
                        condition.status = temp2->value();
                    }
                }
            }
            if(string(temp->name()) == "print"){
                has_print = true;
                print = temp -> value();
            }
            if(string(temp->name()) == "action"){
                has_move = true;
                string buffer = temp -> value();
                move.push_back(buffer);
            }
        }
    }
};
#endif

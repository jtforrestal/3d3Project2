//--------------------------------------------------
//      Neighbour_Table
//-----------------------------------------------

#include <string>
#include <iostream>
#include <stdio.h>
#include "Neigh_Entry.cpp"

class NeighbourTable{
    public:
    Neigh_Entry *top;
    Neigh_Entry *bottom;
    int table_length;

    NeighbourTable();
    void print_table();
    void add_node(std::string name, int port,int cost);
    bool removeNode(std::string name);



};

NeighbourTable::NeighbourTable(){
    top =  NULL;
    bottom =  NULL;
    table_length = 0;
}

void NeighbourTable::print_table(){
    Neigh_Entry *curr;
    curr = top;

    while(curr != NULL){
        std::cout <<"Neighbour Name: " << curr->neigh_name << std::endl << "Neighbour Link Cost: " << curr->neigh_link_cost << std::endl << "Neighbour Port: " << curr->neigh_port << std::endl;
        curr = curr->next_neigh;
    
    }
    
}

void NeighbourTable::add_node(std::string name, int port,int cost){
    Neigh_Entry* new_Entry = new Neigh_Entry(name,port,cost);

    if (bottom == NULL){
        top = bottom = new_Entry;
    }

    else{
        bottom->setNext(new_Entry);
        bottom = new_Entry;
    
    }
    table_length++;
}

bool NeighbourTable::removeNode(std::string name){

    Neigh_Entry* curr =  top;
    Neigh_Entry* prev = top;

    while(curr != 0){
        if(curr->neigh_name == name){
            if (curr == top){
                top = curr->next_neigh;
            }
            else
            {
                prev->setNext(curr->next_neigh);
                if(curr == bottom){
                    bottom = prev;
                }
            }
            delete curr;
            table_length--;
            return true;
            
        }
        prev = curr;
        curr = curr->next_neigh;

    }

    return false;
}
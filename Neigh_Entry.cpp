//-------------------------------------
//      Neigh_Entry
//-------------------------------------

#include <stdio.h>
#include <string>

class Neigh_Entry{
    public:
    Neigh_Entry();
     std::string neigh_name;
     int neigh_link_cost;
     int neigh_port;
     Neigh_Entry *next_neigh;
     void setNext (Neigh_Entry* next);

     Neigh_Entry(std::string neigh_name, int neigh_link_cost, int neigh_port);

};


Neigh_Entry::Neigh_Entry(std::string name,int link_cost, int port){

    neigh_name = name;
    neigh_link_cost = link_cost;
    neigh_port = port;

}

void Neigh_Entry::setNext(Neigh_Entry* next){
    next_neigh = next;
}
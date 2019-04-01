//-------------------------------------
//      DVT_Entry
//-------------------------------------

#include <stdio.h>
#include <string>

class DVT_Entry{
    public:
    DVT_Entry();
    std::string DVT_src_name;
     std::string DVT_dest_name;
     int DVT_link_cost;
     int DVT_port;
     DVT_Entry *next_DVT;
     void setNext (DVT_Entry* next);

     DVT_Entry(std::string src,std::string dest, int DVT_port, int DVT_link_cost);

};


DVT_Entry::DVT_Entry(std::string src, std::string dest,int port, int link_cost ){

    DVT_src_name = src;
    DVT_dest_name = dest;
    DVT_link_cost = link_cost;
    DVT_port = port;
    next_DVT = NULL;

}

void DVT_Entry::setNext(DVT_Entry* next){
    next_DVT = next;
}
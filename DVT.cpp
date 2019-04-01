//--------------------------------------------------
//      DVT_Table
//-----------------------------------------------

#include <string>
#include <iostream>
#include <stdio.h>

#include "DVT_Entry.cpp"

class DVT{
    public:
    DVT_Entry *top;
    DVT_Entry *bottom;
    int table_length;
    std::string Router_name;

    DVT();
    void set_Router_name(std::string name);
    void print_table();
    void add_node(std::string src, std::string dest, int port,int cost);
    void init_node(std::string src, std::string dest, int port,int cost,int init);
    bool removeNode(std::string name);
    int return_length();
    std::string make_packet();



};

DVT::DVT(){
    top =  NULL;
    bottom =  NULL;
    table_length = 0;
}

void DVT::set_Router_name(std::string name){
  Router_name = name;
}

void DVT::print_table(){
    DVT_Entry *curr;
    curr = top;

    while(curr != NULL){
        std::cout <<"DV Source Name: " << curr->DVT_src_name << " DV Dest Name: " << curr->DVT_dest_name  << " DV Link Cost: " << curr->DVT_link_cost <<  " DV Port: " << curr->DVT_port << std::endl;
        curr = curr->next_DVT;
    
    }
    
}


int DVT::return_length(){
    return table_length;
}

void DVT::init_node(std::string src,std::string dest, int port,int cost,int init){
//Prints out content of values recieved
//std::cout<< src <<dest <<port<<cost<<init<<std::endl;

if(init == 1){
  if(src ==  Router_name ){
    DVT_Entry* new_Entry = new DVT_Entry(src,dest,port,cost);

    if (bottom == NULL){
        top = bottom = new_Entry;
    }

    else{
        bottom->setNext(new_Entry);
        bottom = new_Entry;
    
    }
    table_length++;
  }
}
if((init == 0)){
   //For Debugging Purposes
        // std::cout<<"Adding outside of init" <<std::endl;
        //std::cout<< src <<dest <<port<<cost<<init<<std::endl;

    DVT_Entry* new_Entry = new DVT_Entry(src,dest,port,cost);

    if (bottom == NULL){
        top = bottom = new_Entry;
    }

    else{
        bottom->setNext(new_Entry);
        bottom = new_Entry;
    
    }
    table_length++;
  }

}


void DVT::add_node(std::string src,std::string dest, int port,int cost){
  
    DVT_Entry* new_Entry = new DVT_Entry(src,dest,port,cost);

    if (bottom == NULL){
        top = bottom = new_Entry;
    }

    else{
        bottom->setNext(new_Entry);
        bottom = new_Entry;
    
    }
    table_length++;

    std::cout << "Added" <<std::endl;
  //Else dont make the node 
}

bool DVT::removeNode(std::string name){

    DVT_Entry* curr =  top;
    DVT_Entry* prev = top;

    while(curr != 0){
        if(curr->DVT_src_name == name){
            if (curr == top){
                top = curr->next_DVT;
            }
            else
            {
                prev->setNext(curr->next_DVT);
                if(curr == bottom){
                    bottom = prev;
                }
            }
            delete curr;
            table_length--;
            return true;
            
        }
        prev = curr;
        curr = curr->next_DVT;

    }

    return false;
}

std::string DVT::make_packet(){

    DVT_Entry *curr;
    curr = top;
    int port; // Message Port Number
    int link;//message link cost
    std::string dest; // message destination cost
    std::string message = ""; //Final Message

    while(curr != NULL){
      //  std::cout << message << std::endl;
        port =  curr->DVT_port;
        link = curr->DVT_link_cost;
        dest = curr->DVT_dest_name;

        std::string link_string = std::to_string(link);
        std::string port_string = std::to_string(port);

        message = message + "TYPE:CTRL\nSrc_Port:"+Router_name+"\nPort_Name:" +dest +"\nPort_Cost:"+link_string+"\nPort_Number:"+ port_string +"\n";      

        curr =  curr->next_DVT;   
    }
        message = message + "TYPE:NULL";
        return message;
    
}
#include <iostream>
#include <string>
#include<fstream>
#include<string>
#include<iomanip>
#include <cstdlib>
#include "DVT.cpp"
#include "DVT_Entry.cpp"
#include <time.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <cstdbool>
class output{
    public:
void write_Out(std::string src,std::string dest, std::string cost,std::string port);
bool is_file_exist(std::string fileName);

};
//#include "Neighbour_Table.cpp"

//#include "Bellman_Ford.cpp"
//This function will be called in bellman ford after every DV update

//In Bellman Ford function, parse the CTRL type data packet and send that parsed data into the write_out function
//timestamped progression

void output::write_Out(std::string src,std::string dest, std::string cost,std::string port)
{

std::cout<<"DV update : "<< src <<","+dest<<","+cost<<","<< port<<";"<< std::endl;

// declaring argument of time() 
    time_t my_time = time(NULL); 
    std::cout<<ctime(&my_time);

std::string Router_name;
DVT_Entry *top;
DVT_Entry *bottom;
DVT_Entry *curr;
curr = top;

Router_name=src ; //curr->DVT_src_name;

std::string fileName = " routing-output"+Router_name+".txt";
std::ofstream newFile(fileName);

if(is_file_exist(fileName)==true){

newFile.open(fileName, std::ios::app);
newFile<<"DV Source Name: " << curr->DVT_src_name<<std::endl;
    while(curr != NULL){
        newFile<<" DV Dest Name: " << curr->DVT_dest_name  << " DV Link Cost: " << curr->DVT_link_cost << std::endl;
        curr = curr->next_DVT;
    
    }



}

//std::ofstream newFile(fileName);
else{

if(newFile.is_open()) {
    newFile<<"DV Source Name: " << curr->DVT_src_name<<std::endl;
    while(curr != NULL){
        newFile<<" DV Dest Name: " << curr->DVT_dest_name  << " DV Link Cost: " << curr->DVT_link_cost << std::endl;
        curr = curr->next_DVT;
    
    }}

else
{
    std::cout<<"--------------couldn't open file!---------------";
}

}}
bool output::is_file_exist(std::string fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

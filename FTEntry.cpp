//
//  FTEntry.cpp
//  
//
//  Created by Sean Murray on 31/03/2019.
//

#include "FTEntry.hpp"

FTEntry::FTEntry(){}
FTEntry::FTEntry(std::string final_dest,
                 int next_hop_port,
                 std::string next_hop_name)
:
ft_dest{final_dest},
ft_next_port{next_hop_port},
ft_next_name{next_hop_name},
nextPtr{ nullptr } {}

std::string FTEntry::get_final_dest() { return ft_dest; }
int         FTEntry::get_nextport() {return ft_next_port;}
std::string FTEntry::get_nextname() {return ft_next_name;}

void FTEntry::set_finaldest(std::string dest){ft_dest = dest;}
void FTEntry::set_nextport(int port){ft_next_port = port;}
void FTEntry::set_nextname(std::string nextname){ft_next_name = nextname;}

void  FTEntry::printFTEntry(){
    std::cout<< ft_dest << ": " << ft_next_port << " "<< ft_next_name << "\n";
}
FTEntry::~FTEntry(){}

//
//  FTEntry.hpp
//  
//
//  Created by Sean Murray on 31/03/2019.
//

#ifndef FTEntry_hpp
#define FTEntry_hpp

class ForwardTable;

#include<string>
#include<iostream>



class FTEntry
{
    friend class ForwardTable;
    
public:
    FTEntry();
    FTEntry(std::string final_dest,
            int next_hop_port,
            std::string next_hop_name);
    
    // can also return routes from
    std::string get_final_dest();
    int get_nextport();
    std::string get_nextname();
    
    void set_finaldest(std::string dest);
    void set_nextport(int port);
    void set_nextname(std::string nextname);
    
    void printFTEntry();
    ~FTEntry();
    
private:
    std::string ft_dest;
    int ft_next_port;
    std::string ft_next_name;
    FTEntry* nextPtr;
};

#endif /* FTEntry_hpp */

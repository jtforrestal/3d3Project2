//
//  ForwardTable.hpp
//  
//
//  Created by Sean Murray on 31/03/2019.
//

#ifndef ForwardTable_hpp
#define ForwardTable_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include "DVTEntry.h"


class ForwardTable{
private:
    FTEntry* firstPtr;
    FTEntry* lastPtr;
    FTEntry* getNewNode(std::string finaldest,
                        int nextport,
                        std::string nextname) {
        return new FTEntry{dest, nextport, nextname};
    }
public:
    ForwardTable();
    //Destructor for the route list
    ~ForwardTable();
    
    // insert a route object at the front of the list
    void insertAtFront(std::string dest, int nextport, std::string nextname);
    // insert a route object at the back
    void insertAtBack(std::string dest, int nextport, std::string nextname);
    // load the route object into value and the nremove it from the list
    bool removeFromFront();
    // load the route object into the value and remove from the list
    bool removeFromBack();
    // check if the lis tis empty
    bool isEmpty()const    ;
    
    // load entry number entry into value and return true on success
    // count from 0
    bool getByIndex(int& index, std::string& dest, int& nextport, std::string& nextname)const ;
    
    // search based on destination name, takes string and route, returns false if not found
    // if found returns true and fills value with the whole route
    // All things going to plan each table should have only one of each destination
    bool searchDest(std::string term, std::string& dest, int& nextport, std::string& nextname);
    
    //Set destination to port set a new port
    bool setPortforDest( std::string dest, std::string& nextname);
    // Overloaded function to return whole route if needed
    bool setPortforDest(  std::string dest, int nextport,  std::string& nextname);
    
    //Set destination to port set a new port
    bool set_nexthop( std::string dest, std::string& nextname);
    // Overloaded function to return whole route if needed
    bool set_nextname( std::string dest, int& port, int cost);
    //############
    //######This method sets the pointer value to point to the entry object in the list
    //######We may use this to access the entry directly
    bool getPtrforDest(  FTEntry** pointer, std::string dest);
    // prints all objects in a List
    void print()const;
    
    
    
    
};


#endif /* ForwardTable_hpp */

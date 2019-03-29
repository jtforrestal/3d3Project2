#include "DVTEntry.h"
  DVTEntry::DVTEntry(){}
  DVTEntry::DVTEntry(std::string dest, int port, int cost)
  : dvt_dest{dest},dvt_port{port},dvt_cost{cost}, nextPtr{ nullptr } {}

  std::string  DVTEntry::getDest() { return dvt_dest; }
  int  DVTEntry::getPort() {return dvt_port;}
  int  DVTEntry::getCost() {return dvt_cost;}

  void  DVTEntry::setDest(std::string dest){dvt_dest = dest;}
  void  DVTEntry::setPort(int port){dvt_port = port;}
  void  DVTEntry::setCost(int cost){dvt_cost = cost;}

  void  DVTEntry::printDVTEntry(){
    std::cout<< dvt_dest << " " << dvt_port << " "<< dvt_cost << "\n";
  }
   DVTEntry::~DVTEntry(){}

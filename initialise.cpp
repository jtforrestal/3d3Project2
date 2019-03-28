#include "initialise.h"
using namespace std;

void initialise(const std::string& name,ListRoute& forwardingTable,const std::string& fileName  ){
  ifstream inGraphData(fileName,ios::in);

  if(!inGraphData){
  std::cout <<"File could not be opnened. \n";
    exit(EXIT_FAILURE);
  }
  string buffer;
  string source;
  string destination;
  int destPort;
  int linkCost;
  while(inGraphData.good()){
		getline(inGraphData, source, ',');
    getline(inGraphData, destination,',');
    getline(inGraphData, buffer,',');
    destPort = stoi(buffer);
    getline(inGraphData, buffer,'\n');
    linkCost = stoi(buffer);

      if(source == name ){
        route toTable(destination, destPort, linkCost);
        forwardingTable.insertAtBack(toTable);
        }
      }
  }

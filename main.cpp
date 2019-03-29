#include <iostream>
#include <string>
#include<fstream>
#include<string>
#include<iomanip>
#include <cstdlib>
#include "DVT.h"
#include "DVTEntry.h"
#include "initialise.h"
using namespace std;

int main()
{
	DVT forwardingTable;
	string routerName;

  cout << "Which Router is this ?" << endl;
  cin >> routerName;
	string fileName = "graph.csv";
	initialise(routerName, forwardingTable, fileName);
	forwardingTable.print();

	DVTEntry* PtrHolder;

	forwardingTable.getPtrforDest(&PtrHolder,"E");

	PtrHolder->setPort(999);
	cout << "&Holder: "<< &PtrHolder << "*holder: "<< PtrHolder->getPort() <<endl;
	return 0;
}

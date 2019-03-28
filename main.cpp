#include <iostream>
#include <string>
#include<fstream>
#include<string>
#include<iomanip>
#include <cstdlib>
#include "ListRoute.h"
#include "route.h"
#include "initialise.h"
using namespace std;

int main()
{
	ListRoute forwardingTable;
	string routerName;

  cout << "Which Router is this ?" << endl;
  cin >> routerName;
	string fileName = "graph.csv";
	initialise(routerName, forwardingTable, fileName);
	forwardingTable.print();

	route* PtrHolder;

	forwardingTable.getPtrforDest(&PtrHolder,"E");

	PtrHolder->setPort(999);
	cout << "&Holder: "<< &PtrHolder << "*holder: "<< PtrHolder->getPort() <<endl;
	return 0;
}

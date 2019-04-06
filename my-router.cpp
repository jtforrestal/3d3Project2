//
//  my-router.cpp
//
//
//  Created by Sean Murray on 02/04/2019.
//

// changed the int port number to char*
// in order to have a proper input to the address function

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>

#include <string>
#include <fstream>
#include <iostream>
#include <limits.h>

#include <chrono>
#include <future>
#include <sstream>

#include <iterator>
#include <map>

//#define MYPORT "4900" -> passed as main() argument
#define DESTPEER "4951" // hardcoded for now...
#define MAXBUFLEN   2048

const int NEIGHBOUR_UPDATE_TIMEOUT = 5; // if no recv for 5s, ping neighbours

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

typedef std::map<char, int> DV_MAP;

// MAPS + STRUCTS for tables

struct
neighbour
{
    int cost;
    char *port;
    DV_MAP distancevectors;
};

typedef std::map<char, neighbour>   N_MAP;      // <name, node>
typedef std::map<char, char*>       FT_MAP;     // <finaldest, nextport>
typedef std::pair<char, neighbour>  N_PAIR;     // <name, node>
typedef std::pair<char, int>        DV_PAIR;    // <finaldest, totalcost>
typedef std::pair<char, char*>      FT_PAIR;    // <finaldest, nextport>

void bellmanford(N_MAP ntable, DV_MAP *currDV, FT_MAP *ftable);
bool dvupdate   (char thisnode, char nodeX, DV_MAP newtable, N_MAP *ntable, DV_MAP *owntable, FT_MAP *ftable);

std::string dvtostring (DV_MAP newtable, std::string nodename);
DV_MAP      stringtodv (std::string message, char* srcnode);

int main(int argc, char *argv[])
// eg
// ./server [myport#] [destport#] - will be ./server [name]

{
    assert(argc > 1); // ensure program called w/ arguments

    // -----------------------------------------------------------
    //                  IMPORTANT NODE VARIABLES
    // -----------------------------------------------------------


    char    *nodename = argv[1];    // NAME OF NODE
    char    *nodeport = NULL;       // PORT OF NODE
    DV_MAP  nodeDVs;                // NODE'S DISTANCE VECTOR TABLE <dests, distances>
    FT_MAP  nodeFT;                 // NODE'S FORWARDING TABLE <dests, nextports>
    N_MAP   neighbourtable;         // NODE'S NEIGHBOUR INFO <neighb, neighb-info>



    // -----------------------------------------------------------
    //                      INITIALISATION
    // -----------------------------------------------------------




    std::ifstream inFile("graph.csv",std::ios::in);

    if(!inFile) { std::cout<< "Error:infile"; exit(EXIT_FAILURE);}

    std::string buffer;
    std::string source;
    std::string dest;
    char* destPort;
    int   linkCost;

    bool flag = 1;

    while(inFile.good()){

        getline(inFile, source, ',');
        getline(inFile, dest,',');
        getline(inFile, buffer,',');
        destPort = strdup(buffer.c_str());
        getline(inFile, buffer,'\n');
        linkCost = stoi(buffer);

        if(source == nodename){

            // insert to forward table
            nodeFT.insert(FT_PAIR(dest[0], destPort));

            // insert to DVs table
            nodeDVs.insert(DV_PAIR(dest[0], linkCost));

            // make neighbour node, and insert it to neighbour table
            neighbour n;
            n.cost = linkCost;
            n.port = destPort;
            //n.distancevectors = NULL;
            neighbourtable.insert(N_PAIR(dest[0], n));

        }

        if(dest == nodename && flag) {// this is our portneighbourtable.insert(N_PAIR(dest[0], n));

            nodeport = destPort;
            std::cout << "\nFound nodeport: " << nodeport << std::endl;
            flag = 0;
        }
        //free(destPort);
    }

    assert(nodeport!=NULL); // ensure nodeport is initialised

    // -----------------------------------------------------------
    //                 PRINT INITIAL NODE STATE
    // -----------------------------------------------------------


    // printing neighbours
    N_MAP::iterator itrN;
    std::cout << "\nNeighbour-table for " << nodename << ": \n\n";
    std::cout << "\tNeigh\tCost\tPort\tNeighDV\n";
    for (itrN = neighbourtable.begin(); itrN != neighbourtable.end(); ++itrN) {
        std::cout << '\t' << itrN->first
        << '\t' << itrN->second.cost << '\t'<< itrN->second.port << '\t' << "n/a" << "\n\n";
    }
    // printing FT
    FT_MAP::iterator itrFT;
    std::cout << "\nForward-table for " << nodename << ": \n\n";
    std::cout << "\tNeigh\tPort\n";
    for (itrFT = nodeFT.begin(); itrFT != nodeFT.end(); ++itrFT) {
        std::cout << '\t' << itrFT->first
        << '\t' << itrFT->second << "\n\n";
    }



    // -----------------------------------------------------------
    //                  SET UP LISTENING SOCKET
    // -----------------------------------------------------------

    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    ssize_t numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, nodeport, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        // allow others to reuse the address
        int yes = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            return 1;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }


    // *** NOT SURE ABOUT FREEING ADDRINFO HERE...
    freeaddrinfo(servinfo);

    // -----------------------------------------------------------
    //                  BEGIN LISTENING FOR() LOOP
    // -----------------------------------------------------------

    addr_len = sizeof their_addr;

    // set up future with recvfrom() function
    std::future<ssize_t> fut = std::async(recvfrom, sockfd, buf, MAXBUFLEN-1 , 0,
                                          (struct sockaddr *)&their_addr, &addr_len);

    std::chrono::seconds span (NEIGHBOUR_UPDATE_TIMEOUT);


    for(;;) {

        std::cout << std::endl << nodename << ": waiting to recvfrom...\n";

        // -----------------------------------------------------------
        //                      PING/recvfrom() TIMEOUT
        // -----------------------------------------------------------

        // 5s timeout
        // exit while() loop when either:
        // 1. recvfrom() returns a message
        // 2. the 5s 'span' is over

        // if timespan is over -> no recv in 5s -> ping neighbours
        // if recv -> update: table then ping neighbours
        //         -> datagram: send on, don't update neighbours
        //              (maybe we should ping neighbours after recv dgram?)


        // if...
        // (timeout)

        if(fut.wait_for(span)==std::future_status::timeout)
        {
            std::cout << nodename << ": recvfrom() timeout! pinging neighbours...\n\n";



            // -----------------------------------------------------------
            //              PING NEIGHBOURS WITH CURRENT TABLE:
            // -----------------------------------------------------------

            std::cout << "Sent message to";

            //std::string ping_msg = "DV update";

            std::string msg =dvtostring(nodeDVs,nodename);

            //std::cout <<"Message = " << mess <<std::endl;
//            std::string ping_msg = s + " TYPE:CTRL " + itrN->first + " on port: " + itrN->second.port + '\n';


            for (itrN = neighbourtable.begin(); itrN != neighbourtable.end(); ++itrN) {

                //std::string DESTPEER = argv[2];
                // find DESTPEER port in neighbour table....

                // std::string ping_msg = nodename + " TYPE:CTRL " + itrN->first + " on port: " + itrN->second.port + '\n';

                if ((rv = getaddrinfo("localhost", itrN->second.port, &hints, &servinfo)) != 0) {
                    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                    return 1;
                }
                // loop through all the results and make a socket(unused?) -> get address
                for(p = servinfo; p != NULL; p = p->ai_next) {
                    if (socket(p->ai_family, p->ai_socktype,
                               p->ai_protocol) == -1) {
                        perror("talker: socket");
                        continue;
                    }
                    break;
                }

                if (p == NULL) {
                    fprintf(stderr, "talker: failed to create socket\n");
                    return 2;
                }

                if ((numbytes = sendto(sockfd, msg.c_str(), msg.length(), 0,
                                       p->ai_addr, p->ai_addrlen)) == -1) {
                    perror("talker: sendto");
                    exit(1);
                }

                std::cout << " " << itrN->first;

            } // updated all neighbours

            //std::cout << ":\n" << ping_msg << "\n\n";

            continue;
        }



        // else(no timeout)... message received!

        else {

            // -----------------------------------------------------------
            //                      MESSAGE RECEIVED
            // -----------------------------------------------------------

            //ssize_t
            numbytes = fut.get();
            if (numbytes == -1) { perror("recvfrom"); return 9;}

            // eventual objectives on recvfrom():
            //  CTRL - update table, ping neighbours with update
            //  DATA - lookup table, forward packet on


            std::cout << nodename << ": packet origin: " <<
                   inet_ntop(their_addr.ss_family,
                             get_in_addr((struct sockaddr *)&their_addr),
                             s, sizeof s) << std::endl;
            std::cout << nodename << ": packet length: " << (int)numbytes << "\n";
            buf[(int)numbytes] = '\0';
            std::cout << "What?"<<"\n";
           // std::cout << nodename << ": message: \n" << buf << std::endl << std::endl;



            //------------------------------------------------------------
            //                  Parsing the Message for Type
            //------------------------------------------------------------

            // Either CTRL (Control message) or DATA (Datagram Message)

            //The data that is recived in the buffer gets stored in string recvd_message
            std::string recvd_message = buf;
            std::cout << " The Recv is " << recvd_message<< "\n\n\n\n\n";
            //Extract the type of message, this removes the "Type:" from the recvd_message
            int position_1 = recvd_message.find(":");
            recvd_message.erase(0,position_1+1);
            //In the new code, the find function will search for a new line "\n" however for testing, I am using a ":" in the message
            int position_2 = recvd_message.find("\n");
            std::string type_message = recvd_message.substr(0,position_2); //Make a substring called type_message that stores either "CTRL" or "DATA" depending on the packet
            //std::cout << "Type: " << type_message << std::endl; //Print out to the Screen
            std::cout << "Message type is "<< type_message << std::endl;
            //Extract the body of the message
            recvd_message = recvd_message.erase(0, position_2+1); //Remove the Type of message header from the string

           // std::cout <<"Type of Message = " << type_message <<std::endl;

            if(type_message == "CTRL"){
                // so we've received an updated DV from our neighbour

                char source;
                DV_MAP recvdDVs;

                recvdDVs = stringtodv(recvd_message, &source);

                 // print out DV table
                DV_MAP::iterator itrDV;
                std::cout << "\nDV-table for " << source << ": \n\n";
                std::cout << "Dest\tTot Cost\n";
                for (itrDV = recvdDVs.begin(); itrDV != recvdDVs.end(); ++itrDV) {
                    std::cout << itrDV->first;
                    if(itrDV->second==INT_MAX) std::cout << '\t' << "\u221E"<< "\n"; // if infinity
                    else std::cout << '\t' << itrDV->second << "\n";
                }
                std::cout << std::endl;


                // dv update returns 1 if there are changes made
                // ...to our neighbour table distancevectors

                if(dvupdate(nodename[0], source, recvdDVs, &neighbourtable, &nodeDVs, &nodeFT)) {

                    std::cout << "table updated! now need to perform bellman-ford...\n";

                    // update own DV & forward table... (bellman-ford)

                    bellmanford(neighbourtable, &nodeDVs, &nodeFT);

                    // FINISHED SESSION... ENDING NOTES
                    // - if there are new nodes found in an update from a neighbour node
                    // .... how do we pass them into the DV or FT?
                    // .... -> how about, in the dvupdate(), add new nodes w/ MAXSIZE, NULL etc.
                    // ....... -> if they're initialised already, they will be easy to update in BF

                }

                // print out DV table
                //DV_MAP::iterator itrDV;
                std::cout << "\nDV-table for " << source << ": \n\n";
                std::cout << "Dest\tTot Cost\n";
                for (itrDV = recvdDVs.begin(); itrDV != recvdDVs.end(); ++itrDV) {
                    std::cout << itrDV->first;
                    if(itrDV->second==INT_MAX) std::cout << '\t' << "\u221E"<< "\n"; // if infinity
                    else std::cout << '\t' << itrDV->second << "\n";
                }
                std::cout << std::endl;

                // printing FT
                FT_MAP::iterator itrFT;
                std::cout << "\nForward-table for " << nodename << ": \n\n";
                std::cout << "\tNeigh\tPort\n";
                for (itrFT = nodeFT.begin(); itrFT != nodeFT.end(); ++itrFT) {
                    std::cout << '\t' << itrFT->first
                    << '\t' << itrFT->second << "\n";
                }

            }




            // -----------------------------------------------------------
            //                  DATA: FORWARD MESSAGE ON
            // -----------------------------------------------------------

            // how do we find the address of the node we want to send to?
            //  -> use socket() to get the address of the destination port...

            // using hardcoded port: DESTPEER "4951"...
            //  (in future, this port# will be found using DV or lookup table)
            // now use socket()...
            //  -> to get the address we need: [p->ai_addr, p->ai_addrlen]
            if(type_message == "DATA"){
              std::cout<< "WE made it to DATA"<< std::endl;
              //--------------------------------------
              //     Initial Source of packet
              //--------------------------------------
              int position_ctrl_1 = recvd_message.find("\n");
              std::string srcname = recvd_message.substr(0,position_ctrl_1);
              recvd_message.erase(0,position_ctrl_1+1);


                //--------------------------------------
                //     Final Destination Router Name
                //--------------------------------------
                int position_ctrl_2 = recvd_message.find(",");
                std::string destname = recvd_message.substr(0,position_ctrl_2);
                recvd_message.erase(0,position_ctrl_2+1);


                //--------------------------------------
                //        Destination Router Cost
                //--------------------------------------

                int position_ctrl_3 = recvd_message.find("\n");
                std::string totalcost_str = recvd_message.substr(0,position_ctrl_3);
                recvd_message.erase(0, position_ctrl_3+1);
                int totalcost = std::stoi(totalcost_str); //Convert to int

                std::string next_hop;

                for (itrFT = nodeFT.begin(); itrFT != nodeFT.end(); ++itrFT) {
                    if(itrFT->first == "destname"){
                      next_hop = itrFT->second;
                    }
                }
                  std::cout<< recvd_message<< "\n";  // output what is left of the packet
                  recvd_message = "Type:DATA\nSrc_Node:"+ srcname +"\n" + destname + ":" +"0"+ recvd_message;
                if ((rv = getaddrinfo("localhost", next_hop.c_str(), &hints, &servinfo)) != 0) {
                   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
                   return 1;
               }
               // loop through all the results and make a socket(unused?) -> get address
                   if (socket(p->ai_family, p->ai_socktype,
                                        p->ai_protocol) == -1) {
                       perror("talker: socket");
                       continue;
                   }
                   break;


               if (p == NULL) {
                   fprintf(stderr, "talker: failed to create socket\n");
                   return 2;
               }

               // sending from the current server socket...
               // back to the address of server2

               if ((numbytes = sendto(sockfd, recvd_message.c_str(),recvd_message.length() , 0,
                                      p->ai_addr, p->ai_addrlen)) == -1) {
                   perror("talker: sendto");
                   exit(1);
               }

            }
            //
            //            if ((rv = getaddrinfo("localhost", DESTPEER, &hints, &servinfo)) != 0) {
            //                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            //                return 1;
            //            }
            //            // loop through all the results and make a socket(unused?) -> get address
            //            for(p = servinfo; p != NULL; p = p->ai_next) {
            //                if (socket(p->ai_family, p->ai_socktype,
            //                                     p->ai_protocol) == -1) {
            //                    perror("talker: socket");
            //                    continue;
            //                }
            //                break;
            //            }
            //
            //            if (p == NULL) {
            //                fprintf(stderr, "talker: failed to create socket\n");
            //                return 2;
            //            }
            //
            //            // sending from the current server socket...
            //            // back to the address of server2
            //
            //            if ((numbytes = sendto(sockfd, buf, MAXBUFLEN-1, 0,
            //                                   p->ai_addr, p->ai_addrlen)) == -1) {
            //                perror("talker: sendto");
            //                exit(1);
            //            }


            // -----------------------------------------------------------
            //                  CTRL: UPDATE, SEND ON UPDATES
            // -----------------------------------------------------------




            // recall recvfrom() function

            fut = std::async(recvfrom, sockfd, buf, MAXBUFLEN-1 , 0,
                             (struct sockaddr *)&their_addr, &addr_len);

        }
        // -----------------------------------------------------------
        //                      END LISTENING FOR LOOP
        // -----------------------------------------------------------

    }


    freeaddrinfo(servinfo);
    close(sockfd);

    return 0;
}

void bellmanford(N_MAP ntable, DV_MAP *currDV, FT_MAP *ftable) {


    // go through our DVs, check if there is a better cost found in our neighbour table

    DV_MAP::iterator itrDV;
    std::cout << "----------------------Bellman ford---------------------------------" << std::endl;

    for(itrDV = currDV->begin(); itrDV != currDV->end(); ++itrDV) {

        char nodeX = itrDV->first; //This is the current Node we are moving through on our DV table

        std::cout << "DV Map for: " << nodeX <<std::endl; //Print out

        // what if some of these are not found?

        int currpathcost = currDV->find(nodeX)->second; //Its current path cost from this router

        std::cout <<"DV Current Path Cost: " << currpathcost << std::endl <<std::endl;

        int DvX;    // neighbour distance to nodeX
        int Cv;     // neighbour link cost
        int newpathcost;

        N_MAP::iterator itrN;

        std::cout <<"-----Neighhbour Table-------" <<std::endl;

        for (itrN = ntable.begin(); itrN != ntable.end(); ++itrN) {

            std::cout << "itrN first: " << itrN->first <<std::endl;

            // check neighbour's DV entry & link cost
            DvX = (itrN->second.distancevectors).find(nodeX)->second;
            Cv = itrN->second.cost;

            std::cout << "DvX for itrN: " << DvX <<std::endl;
            std::cout << "Cv for itrN: " << Cv << std::endl;

            newpathcost = Cv + DvX;
            std::cout << "Newpath cost: " <<std::endl;

            std::cout << "newpath cost: " << Cv << "+" << DvX << "=" << newpathcost << std::endl;
            std::cout << "BF for " << nodeX << ": current val: " << currpathcost << ", through " << itrN->first << ":" << newpathcost << std::endl <<std::endl;
            if( currpathcost > newpathcost ) { // update table

                currDV->find(nodeX)->second = newpathcost;
                std::cout << "updated!\n";
                // need to also update forward table!
                // if we've just updated out DV from node itrN
                // we want to add it's port to our forward table


            }
        }

    }
    std::cout << "----------------------End of Bellman ford---------------------------------" << std::endl;
}

// returns 0 if no updates are made

bool dvupdate(char thisnode, char nodeX, DV_MAP newtable, N_MAP *ntable, DV_MAP *owntable, FT_MAP *ftable) {

    DV_MAP oldtable = ntable->find(nodeX)->second.distancevectors;

    char destnode;
    int  newcost;

    DV_MAP::iterator itrDV;

    bool updateflag = false; // any changes to our neighbour DVs?

    for (itrDV = newtable.begin(); itrDV != newtable.end(); ++itrDV) {

        // if old entry, delete it and insert updated entry

        destnode = itrDV->first;
        newcost = itrDV->second;


    // is this a node we haven't seen before?

    if(destnode != thisnode) {

        if(owntable->count(destnode) == 0) {
            owntable->insert(DV_PAIR(destnode, INT_MAX));
        }

        if(ftable->count(destnode) == 0) {
            ftable->insert(FT_PAIR(destnode, "n/a"));
        }
    }

    // do we need to update our knowledge of nodeX's DVs ?

        if(oldtable.count(destnode) != 0) {
            // found entry for node X

            if(oldtable.find(destnode)->second == newcost) {
                continue; // duplicate entry, ignore
            }

            std::cout << "erased neighb DV for " << nodeX <<
            ":" << oldtable.find(destnode)->first << " " <<
            oldtable.find(destnode)->second <<"\n";

            // if different cost, erase current pair, replace: insert new pair
            (ntable->find(nodeX)->second.distancevectors).erase(itrDV);
        }

        // if not found, or different cost entry found (and the old one erased)...
        // insert new entry from data
        (ntable->find(nodeX)->second.distancevectors).insert(DV_PAIR(destnode, newcost));

        updateflag = true;

        // debugging -> check it's actually updating the tables

        //std::cout << "ntable: updated DV for " << nodeX <<
          //  ":" << destnode << " " << newcost <<"\n";
    }

    if(!updateflag) {
        std::cout << "ntable: no updates to DV for " << nodeX << std::endl;
        return 0;
    }

    return 1;
}


std::string dvtostring(DV_MAP newtable, std::string nodename){

  DV_MAP::iterator itrDV;
  std::string message = "Type:CTRL\nSrc_Node:" + nodename + "\n"; //The message we will send;

  for(itrDV = newtable.begin();itrDV != newtable.end();++itrDV){

     char findest = itrDV->first;
    int newdist = itrDV->second;


    std::string newcost = std::to_string(newdist);

    message = message + findest+"," + newcost + "\n";
  }

    message = message + "Z,";
    return message;

}

// example of function call...
// DV_MAP newdvupdate = stringtodv(msg, sourcenode);

DV_MAP stringtodv(std::string recvd_message, char* srcnode){


    // string will be a CTRL message as follows:

    // Src_Node:<srcnode>
    // <findest>,<totalcost>
    // <findest>,<totalcost>
    // ....
    // "\0"

    //*srcnode =

    //std::string srcnode_str(srcnode);
    DV_MAP newtable;

    //-----------------------------------
    //           Source Node
    //----------------------------------

    int position_ctrl_A = recvd_message.find(":");
    recvd_message.erase(0,position_ctrl_A+1);
    int position_ctrl_B = recvd_message.find("\n");
    std::string src_router_name = recvd_message.substr(0,position_ctrl_B);
    recvd_message.erase(0, position_ctrl_B+1);



    *srcnode = src_router_name[0]; // char* = string ??? may need to c_str() or index [0]

    //----------------------------------

    //std::cout << "stringtodv table for " << src_router_name << std::endl;

    while(true){

        //--------------------------------------
        //     Final Destination Router Name
        //--------------------------------------
        int position_ctrl_1 = recvd_message.find(",");
        std::string destname = recvd_message.substr(0,position_ctrl_1);
        recvd_message.erase(0,position_ctrl_1+1);

        if(destname == "Z") {
        break;
        }

        //--------------------------------------
        //        Destination Router Cost
        //--------------------------------------

        int position_ctrl_2 = recvd_message.find("\n");
        std::string totalcost_str = recvd_message.substr(0,position_ctrl_2);
        recvd_message.erase(0, position_ctrl_2+1);
        int totalcost = std::stoi(totalcost_str); //Convert to int


        //------------------------------------------
        //   Insert <dest,cost> into DV table map
        //------------------------------------------

        newtable.insert(DV_PAIR(destname[0], totalcost));

        //std::cout << destname << "," << totalcost <<std::endl;
    }

  //  std::cout << std::endl;

    return newtable;

  }

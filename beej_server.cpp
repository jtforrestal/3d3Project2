// CHANGES FROM LAST VERSION:
//  - pass 'MYPORT' in as main argument eg, ./server [port#]

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <thread>

#include <sstream>
#include <chrono>
#include <future>

//--------------------------------------
//          Included files
//--------------------------------------
#include "DVT.cpp"
#include "Neighbour_Table.cpp"


//--------------------------------------
//              constants
//--------------------------------------

//#define MYPORT "4900" -> passed as main() argument
#define DESTPEER "4951" // hardcoded for now...
#define MAXBUFLEN 100

const int NEIGHBOUR_UPDATE_TIMEOUT = 5; // if no recv() for 5s, ping neighbours


//--------------------------------------
//              functions
//--------------------------------------

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//-------------------------------------------------
//
//                     MAIN
//
//-------------------------------------------------

int main(int argc, char *argv[])

// eg
// ./server [port#]

{

    //-------------------------------------------------
    //          NeighbourTable Example
    //-------------------------------------------------
   //NeighbourTable Table1;
   // Table1.add_node("Test", 3, 5000);
    //Table1.add_node("Test_2",4, 5001);
    //Table1.print_table();
    //std::cout <<"Leaving table";
    
    //-------------------------------------------------
    //                 Initalise Table
    //-------------------------------------------------

	DVT DV_table_A; //Create the DVT table
	std::string routerName = "A"; //Assign the router name

    DV_table_A.set_Router_name(routerName);

    DV_table_A.init_node("A","B",10001,4,1);
    DV_table_A.init_node("A","E",10004,1,1);
    DV_table_A.init_node("B","A",10000,4,1);
    DV_table_A.init_node("B","C",10002,3,1);
    DV_table_A.init_node("B","E",10004,2,1);
    DV_table_A.init_node("B","F",10005,1,1);
    DV_table_A.init_node("C","B",10001,3,1);
    DV_table_A.init_node("C","D",10003,4,1);
    DV_table_A.init_node("C","F",10005,1,1);
    DV_table_A.init_node("D","C",10002,4,1);
    DV_table_A.init_node("D","F",10005,3,1);
    DV_table_A.init_node("E","A",10000,1,1);
    DV_table_A.init_node("E","B",10001,2,1);
    DV_table_A.init_node("E","F",10005,3,1);
    DV_table_A.init_node("F","B",10001,1,1);
    DV_table_A.init_node("F","C",10002,1,1);
    DV_table_A.init_node("F","D",10003,3,1);
    DV_table_A.init_node("F","E",10004,3,1);
    
    
    std::string A_DVT;
    A_DVT = DV_table_A.make_packet();
 

//    std::string fileName = "graph.csv"; //Load in the initial table
//	initialise(routerName, DV_table_A, fileName); //Call initialise function
	DV_table_A.print_table(); // Print DVT table

    
    // -----------------------------------------------------------
    //                  SET UP LISTENING SOCKET
    // -----------------------------------------------------------
    
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

   // Packet P1;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    if ((rv = getaddrinfo(NULL, "5000", &hints, &servinfo)) != 0) {
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
    
    freeaddrinfo(servinfo);
    
    // -----------------------------------------------------------
    //                      TIMEOUT SET UP
    // -----------------------------------------------------------
    
    // Span of 5 seconds
    // The future monitors the output of the recvfrom() function
    
    addr_len = sizeof their_addr;
    
    std::chrono::seconds span (NEIGHBOUR_UPDATE_TIMEOUT);
    
    std::future<ssize_t> fut = std::async(recvfrom, sockfd, buf, MAXBUFLEN-1 , 0,
                                          (struct sockaddr *)&their_addr, &addr_len);
    
    
    //  *** NOT SURE IF THIS IS IN THE RIGHT PLACE? ***
    //-------------------------------------------------e-----------
    //    Making a For Loop to store the forwarding table data
    //------------------------------------------------------------
    
    
    
    // -----------------------------------------------------------
    //                  BEGIN LISTENING FOR() LOOP
    // -----------------------------------------------------------
    
    for(;;) {
        
        printf("waiting to recvfrom...\n");

        
        // if
        // (no recv after 5s)... timeout
        
        if(fut.wait_for(span)==std::future_status::timeout)
        {
            
            // -----------------------------------------------------------
            //                      PING/recvfrom() TIMEOUT
            // -----------------------------------------------------------
            
            
            std::cout << "recvfrom() timeout: pinging neighbours...\n\n";
            
            
            // -----------------------------------------------------------
            //              PING NEIGHBOURS WITH CURRENT TABLE:
            // -----------------------------------------------------------
            
            
            
            
            std::string ping_msg = "TYPE:DATA\nDV update";
            
            //std::string DESTPEER = argv[2];
            // find DESTPEER port in neighbour table....
            
            if ((rv = getaddrinfo("localhost", "5001", &hints, &servinfo)) != 0) {
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
            
            if ((numbytes = sendto(sockfd, ping_msg.c_str(), ping_msg.length(), 0,
                                   p->ai_addr, p->ai_addrlen)) == -1) {
                perror("talker: sendto");
                exit(1);
            }
            
            
            continue; // loop round
        }
        
        
        
        
        
        // else
        //  no timeout...

        
        // -----------------------------------------------------------
        //                  MESSAGE RECEIVED
        // -----------------------------------------------------------
    
        printf("listener: got packet from %s\n",
               inet_ntop(their_addr.ss_family,
                         get_in_addr((struct sockaddr *)&their_addr),
                         s, sizeof s));
        printf("listener: packet is %d bytes long\n", numbytes);
        buf[numbytes] = '\0';
        printf("listener: packet contains \"%s\"\n", buf);
        std::cout << std::endl;

    
     
        //------------------------------------------------------------
        //                  Parsing the Message for Type
        //------------------------------------------------------------
        //Either CTRL (Control message) or DATA (Datagram Message)
        

        //The data that is recived in the buffer gets stored in string recvd_message
        std::string recvd_message = buf;

        //Extract the type of message, this removes the "Type:" from the recvd_message
        int position_1 = recvd_message.find(":");
        recvd_message.erase(0,position_1+1);
        
        //In the new code, the find function will search for a new line "\n" however for testing, I am using a ":" in the message
        int position_2 = recvd_message.find("\n");
        std::string type_message = recvd_message.substr(0,position_2); //Make a substring called type_message that stores either "CTRL" or "DATA" depending on the packet
       //std::cout << "Type: " << type_message << std::endl; //Print out to the Screen
        

        //Extract the body of the message
        recvd_message = recvd_message.erase(0, position_2+1); //Remove the Type of message header from the string
       //std::cout << "Content of Message: " << std::endl;
       // std::cout << recvd_message << std::endl;



      //  std::cout << std::endl;   


        if(type_message == "CTRL"){

            int position_ctrl_1 = recvd_message.find(":");
            recvd_message.erase(0,position_ctrl_1+1);
            int position_ctrl_2 = recvd_message.find("\n");
            std::string dest_router_name = recvd_message.substr(0,position_ctrl_2);
            recvd_message.erase(0, position_ctrl_2);

            

            //Parse Cost Function                                                                                                                                                                           

            int position_ctrl_3 = recvd_message.find(":");
            recvd_message.erase(0, position_ctrl_3);
            int position_ctrl_4 = recvd_message.find("\n");
            std::string dest_router_cost = recvd_message.substr(0,position_ctrl_4);
            recvd_message.erase(0,position_ctrl_4);

            //Parse Port Number                                                                                                                                                                             

            int position_ctrl_5 = recvd_message.find(":");
            recvd_message.erase(0,position_ctrl_5+1);
            int position_ctrl_6 = recvd_message.find("\n");
            std::string next_hop_router_port = recvd_message.substr(0,position_ctrl_6);
            recvd_message.erase(0,position_ctrl_6);

           // std::cout << "Router Name: " << dest_router_name << " Router Cost: " <<router_cost << " Router Port: " << router_port << std::endl;

           //int dest_cost = std::stoi(dest_router_cost);
           //int hop = std::stoi(next_hop_router_port);
           DV_table_A.add_node(routerName,dest_router_name,10001,3);

           DV_table_A.print_table();

        }   

        // -----------------------------------------------------------
        //                  FORWARD MESSAGE TO SERVER 2
        // -----------------------------------------------------------
        
        // how do we find the address of the node we want to send to?
        //  -> use socket() to get the address of the destination port...
        
        // using hardcoded port: DESTPEER "4951"...
        //  (in future, this port# will be found using DV or lookup table)
        // now use socket()...
        //  -> to get the address we need: [p->ai_addr, p->ai_addrlen]
        
        
        if(type_message == "DATA"){
          
             //----------------------------------------------------
             //                Parse Destination IP
             //----------------------------------------------------
            std::string Dest_IP = recvd_message;

            int position_3 = Dest_IP.find("\n");
            Dest_IP = Dest_IP.substr(0,position_3);
            std::cout << "Destination" << Dest_IP << std::endl;
            recvd_message = recvd_message.erase(0,position_3);



            // 1)Look up routing table
            // 2)Perform bellman-ford algorithm
            // 3)Send on the message to nearest neighbhour
        
        if ((rv = getaddrinfo("localhost", "5001", &hints, &servinfo)) != 0) {
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
        
        // sending from the current server socket...
        // back to the address of server2
         
        std::cout << "Before Sending: " << std::endl;
        std::cout << A_DVT << std::endl;
        std::cout <<"Length: " << A_DVT.length() << std::endl;
       
        if ((numbytes = sendto(sockfd, A_DVT.c_str(), A_DVT.length(), 0,
                               p->ai_addr, p->ai_addrlen)) == -1) {
            perror("talker: sendto");
            exit(1);
       
        }
        
        std::cout << "P->ai_adder: " << p->ai_addr << "   P->ai_addrelen: " << p->ai_addrlen << std::endl << std::endl;

        freeaddrinfo(servinfo);
            if ((rv = getaddrinfo("localhost", DESTPEER, &hints, &servinfo)) != 0) {
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
            
            // sending from the current server socket...
            // back to the address of server2
            
            if ((numbytes = sendto(sockfd, buf, MAXBUFLEN-1, 0,
                                   p->ai_addr, p->ai_addrlen)) == -1) {
                perror("talker: sendto");
                exit(1);
            }
            
            std::cout << "P->ai_adder: " << p->ai_addr << "   P->ai_addrelen: " << p->ai_addrlen << std::endl << std::endl;

            freeaddrinfo(servinfo);
             
            
            
            // reset recvfrom() listening function/timeout
            
            fut = std::async(recvfrom, sockfd, buf, MAXBUFLEN-1 , 0,
                             (struct sockaddr *)&their_addr, &addr_len);
             
             
         }
    // -----------------------------------------------------------
    //                      END LISTENING FOR LOOP
    // -----------------------------------------------------------
    }
    
    close(sockfd);
    
    return 0;
}


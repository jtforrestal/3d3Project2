// basic change

// BEEJ'S UDP CLIENT FILE

/*
 ** talker.c -- a datagram "client" demo
 */

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
#include <string>



#define MYPORT "5000"

//#define SERVERPORT "4900" -> now passed in main()

int main(int argc, char *argv[])
// eg
// ./client [destport#]

{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    
    if (argc != 3) {
        fprintf(stderr,"usage: [destport] [message]\n");
        exit(1);
    }
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    
    
    // ---------------------------------
    //  MYPORT SOCKET SETUP - TESTING
    // ---------------------------------
    
    
    if ((rv = getaddrinfo("localhost", MYPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    int mysocketfd;
    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((mysocketfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        
        break;
    }
    
    
    // ---------------------------------
    
    
    
    
    
    // pass in first main() argument argv[1] - the server port
    if ((rv = getaddrinfo("localhost", argv[1], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    
      std::string Test_Message = "TYPE:DATA\nPort_Name:F\nPort_Cost:5\nPort_Number:5000\nThis content is data";
    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        
        break;
    }
    
    
    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }
  
    if ((numbytes = sendto(mysocketfd, Test_Message.c_str(), strlen(Test_Message.c_str()), 0,
                           p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }
    
    freeaddrinfo(servinfo);
    
    printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
    close(sockfd);
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include "Packet.h"


void error(const char* msg)
{
    perror(msg);
    exit(0);
}

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    // open a connection
    if (argc < 3)
    {
        std::cerr<< "usage hostname port\n" << std::endl;
        return 0;
    }
    
    int portno = atoi(argv[2]);
    int sockfd = socket(AF_INET,SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    
    struct hostent *server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        std::cerr << "usage " << argv[0] << " hostname port" << std::endl;
        return 0;
    }
    
    struct sockaddr_in serv_addr;
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
          (char *) &serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
        error("ERROR connecting");
    
    // read input from stdin

    dxl_comm_packet pack;

    for(int i=0; i<NUMBER_OF_MOTORS; i++)
    {
        while (true)
        {
            int pos;
            std::cout << "Enter a number from 0 to 4096. ";
            std::string input;
            getline(std::cin, input);
            if (std::stringstream(input) >> pos && pos >= 0 && pos <= 4096)
            {
                pack[2*i] = i;
                pack[2*i+1] = pos;
                break;
            } 
                    
            std::cerr << "Not a valid integer. (0 <= pos <= 4096)" << std::endl;
        }
    }

    std::cout << pack[0] << std::endl;
    std::cout << pack[1] << std::endl;
    std::cout << pack[2] << std::endl;
    std::cout << pack[3] << std::endl;
    std::cout << PACKET_SIZE << std::endl;    

    // send packet
    int n = write(sockfd,pack,PACKET_SIZE);
    if ( n < 0)
        error("ERROR problem writing to socket");
    else
        std::cout << n << " bytes written." << std::endl;
   
    // close connection 
    close(sockfd);
}

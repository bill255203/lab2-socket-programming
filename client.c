#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "header.h"
#include <netinet/in.h>


int main(int argc , char *argv[])
{   
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(45525);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    
    // Send a message to the server
    char message[] = "Hello, server!";
    send(client_socket, message, strlen(message), 0);

    // Receive a message from the server
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes_received] = '\0';
    printf("Received message: %s\n", buffer);

    //uint16_t checksum = calc_segment_checksum(&seg);
    //printf("Checksum = 0x%04x\n", checksum);

    close(client_socket);
    return 0;
    //Create TCP socket.//


    //Set up server's address.//


    //Connect to server's socket.//

    
    //Receive message from server and print it out.//



    //////////////////////////////////////////////////////////
    //                   TASK1(Client)                      //  
    //////////////////////////////////////////////////////////
    // TODO: create a socket and connet to server.          //
    //       (server's IP address = "127.0.0.1")            //
    //       (server's Port number = 45525)                 //
    //                                                      //
    // TODO: Receive 1 message from server and print it out.//
    //       (The message you sent from server)             //
    //////////////////////////////////////////////////////////
    


    ///////////////////////////////////////////////////////////
    //                   TASK2,3(Client)                     //
    ///////////////////////////////////////////////////////////
    // TODO: Instantiate a Segment                           //
    // TODO: Pass your socket_fd and the instance of Segment //
    //       into the receivedata().                         //
    // TODO: Write your own function to create header.       //
    //       (All information is in the Segment instance.    //
    // TODO: Use sendheader(char* header) to send the header //
    //       to server.                                      //
    //                                                       //
    // Example:                                              //
    //                                                       //
    //       Segment s;                                      //
    //       receivedata(sock_fd,&s);                        //
    //       myheadercreater(&s);  //your own function       //
    //       sendheader(sock_fd,s.header);                   // 
    //                                                       //
    //                                                       //
    // Then you will see the result.                         //  
    ///////////////////////////////////////////////////////////
    

    //Segment s;
    //receivedata(socket_fd,&s);
    //sendheader(socket_fd,s.header);
    //close(socket_fd);
}


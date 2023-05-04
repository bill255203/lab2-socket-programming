#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "header.h"
#include <netinet/in.h>

int main(int argc , char *argv[]){
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(45525);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    listen(server_socket, 5);

    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);

    char* message = "Hi, I'm server 109070032";
    send(client_socket, message, strlen(message), 0);

    //call serverfunction to create connection
    serverfunction(client_socket);
    close(server_socket);
    return 0;

    //Create TCP socket//



    //Set up server's address.//



    //Bind socket to the address.//  



    //Listening the socket.//



    //Accept the connect request.//



    //Send message to client.//



    ////////////////////////////////////////////////////////////
    //                   TASK 1(Server)                       //
    ////////////////////////////////////////////////////////////
    // TODO: Create a TCP socket bind to port 45525.          //
    // TODO: Listen the TCP socket.                           //
    // TODO: Accept the connect and get the Client socket     //
    //       file descriptor.                                 //
    // TODO: Send 1 message "Hi, I'm server {Your_student_ID}"//
    //       to client.                                       //
    // Then go finish the client.c TASK 1                     //
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    //                   TASK 2,3(Server)                     //
    ////////////////////////////////////////////////////////////
    // TODO: Pass the client socket fd into serverfuntion()   //
    //                                                        //
    // Example:                                               //
    //           serverfunction(client_fd);                   //
    //                                                        //
    // Then go finish the client.c TASK2,3                    //
    ////////////////////////////////////////////////////////////


}

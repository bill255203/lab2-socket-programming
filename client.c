#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "header.h"
#include <netinet/in.h>
#include <stdint.h>
#include <string.h>

void myheadercreater(Segment* s){
    // Extract the two bytes from s->l4info.SourcePort
    uint16_t source_port = s->l4info.SourcePort;
    uint8_t byte0 = source_port >> 8; 
    uint8_t byte1 = source_port;
    s->header[0] = byte0;
    s->header[1] = byte1;
    // Extract the two bytes from s->l4info.DesPort
    uint16_t dest_port = s->l4info.DesPort;
    uint8_t byte2 = dest_port >> 8; 
    uint8_t byte3 = dest_port; 
    s->header[2] = byte2;
    s->header[3] = byte3;

    uint32_t seq = s->l4info.SeqNum;
    uint8_t byte4 = seq >> 24;
    uint8_t byte5 = seq >> 16;
    uint8_t byte6 = seq >> 8;
    uint8_t byte7 = seq;
    s->header[4] = byte4;
    s->header[5] = byte5;
    s->header[6] = byte6;
    s->header[7] = byte7;

    uint32_t ack = s->l4info.AckNum;
    uint8_t byte8 = ack >> 24;
    uint8_t byte9 = ack >> 16;
    uint8_t byte10 = ack >> 8;
    uint8_t byte11 = ack;
    s->header[8] = byte8;
    s->header[9] = byte9;
    s->header[10] = byte10;
    s->header[11] = byte11;
    s->header[12] = 0x50;
    s->header[13] = 0x10;

    uint16_t win = s->l4info.WindowSize;
    uint8_t byte14 = win >> 8;
    uint8_t byte15 = win & 0xFF;
    s->header[14] = byte14;
    s->header[15] = byte15;

    //task 3 here
    uint16_t com1 = (byte4 << 8) | byte5;
    uint16_t com2 = (byte6 << 8) | byte7;
    uint16_t com3 = (byte8 << 8) | byte9;
    uint16_t com4 = (byte10 << 8) | byte11;
    uint16_t com5 = (s->l4info.HeaderLen << 4);
    uint16_t flag = s->l4info.Flag;
    uint16_t len = s->l4info.HeaderLen;
    uint16_t prot = s->l3info.protocol;
    
    uint64_t sum = 0;
    for (int i = 0; i < 16; i += 2) {
        uint16_t value = (s->l3info.SourceIpv4[i] << 8) | s->l3info.SourceIpv4[i+1]; 
        sum += value; // add the value to the sum
    }
    for (int i = 0; i < 16; i += 2) {
        uint16_t value = (s->l3info.DesIpv4[i] << 8) | s->l3info.DesIpv4[i+1];
        sum += value; // add the value to the sum
    }
    sum += (source_port + dest_port + com1 + com2 + com3 + com4  + flag + len + prot + win)*2 + com5;
    uint64_t carry = (sum & 0xffff000000000000) >> 48; // extract the carry from the high-order bits
    sum = (sum & 0x0000ffffffffffff) + carry;
    uint64_t result = ~sum;

    //print com1 to 8
    printf("%x %x %x %x %x %lx %lx %lx\n", com1, com2, com3, com4, com5, sum, carry, result);

}

int main(int argc , char *argv[])
{   
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(45525);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    // Receive a message from the server
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes_received] = '\0';
    printf("Received message: %s\n", buffer);

    Segment s;
    receivedata(client_socket, &s);
    myheadercreater(&s);
    sendheader(client_socket,s.header);
    close(client_socket);
    return 0;
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
}


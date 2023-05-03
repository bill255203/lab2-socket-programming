#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "header.h"
#include <netinet/in.h>


/*typedef struct {
    // Define the fields of the Segment struct here
    // For example:
    int sequence_number;
    int data_size;
    char data[1024];
    char header[256];
} Segment;

void receivedata(int socket_fd, Segment* segment) {
    // Receive data from the server and store it in the segment struct
    // For example:
    recv(socket_fd, segment->header, sizeof(segment->header), 0);
    recv(socket_fd, segment->data, sizeof(segment->data), 0);
    segment->data_size = strlen(segment->data);
    // Parse the header to fill in the remaining fields of the segment struct
    // For example:
    sscanf(segment->header, "Sequence Number: %d", &segment->sequence_number);
}

void myheadercreater(Segment* segment) {
    // Create a header string based on the fields of the segment struct
    // For example:
    sprintf(segment->header, "Sequence Number: %d, Data Size: %d", segment->sequence_number, segment->data_size);
}

void sendheader(int socket_fd, char* header) {
    // Send the header string to the server
    send(socket_fd, header, strlen(header), 0);
}
*/
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



#include <stdint.h>
#include <string.h>

// calculate the one's complement sum of 16-bit words in the given buffer
uint16_t checksum(uint16_t *buf, size_t len) {
    uint32_t sum = 0;
    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }
    if (len == 1) {
        sum += *(uint8_t*)buf;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (uint16_t)~sum;
}

// calculate the L4 checksum
uint16_t calc_l4_checksum(L4info l4info) {
    uint16_t buf[7];
    buf[0] = (l4info.AckNum >> 16) & 0xffff;
    buf[1] = l4info.AckNum & 0xffff;
    buf[2] = (l4info.SeqNum >> 16) & 0xffff;
    buf[3] = l4info.SeqNum & 0xffff;
    buf[4] = (l4info.SourcePort >> 16) & 0xffff;
    buf[5] = l4info.SourcePort & 0xffff;
    buf[6] = l4info.DesPort & 0xffff;
    return checksum(buf, 14);
}

// calculate the L3 checksum
uint16_t calc_l3_checksum(L3info l3info) {
    uint16_t buf[4];
    uint16_t *ipv4_src = (uint16_t*)l3info.SourceIpv4;
    uint16_t *ipv4_dst = (uint16_t*)l3info.DesIpv4;
    buf[0] = ipv4_src[0];
    buf[1] = ipv4_src[1];
    buf[2] = ipv4_dst[0];
    buf[3] = ipv4_dst[1];
    buf[4] = htons(l3info.protocol);
    buf[5] = 0; // reserved field
    buf[6] = sizeof(L4info);
    return checksum(buf, 12);
}

// calculate the checksum for the entire Segment structure
uint16_t calc_segment_checksum(Segment *seg) {
    uint16_t buf[17];
    memcpy(buf, seg->pseudoheader, 12);
    buf[12] = calc_l3_checksum(seg->l3info);
    buf[13] = calc_l4_checksum(seg->l4info);
    memcpy(buf+14, seg->header, 20);
    return checksum(buf, 34);
}


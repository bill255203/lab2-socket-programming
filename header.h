#pragma once
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct l4info{
    uint32_t AckNum,SeqNum,SourcePort,DesPort,Flag,HeaderLen,WindowSize;
}L4info;

typedef struct l3info{
    char SourceIpv4[16], DesIpv4[16];
    uint32_t protocol;
}L3info;

typedef struct Segment{
    char header[20];
    char pseudoheader[12];
    L3info l3info;
    L4info l4info;
}Segment;


void serverfunction(int clientfd);
void receivedata(int sockfd,Segment* s);
void sendheader(int sockfd, char* header);

void serverfunction(int client_socket) {
    //Pass the client socket fd into serverfuntion()
    int sockfd = client_socket;
    Segment s;
    while (1) {
        //Receive the data from the client
        receivedata(sockfd, &s);
        //Send the data to the client
        sendheader(sockfd, s.header);
    }
}
void receivedata(int sockfd, Segment* s){
    //Receive data from the server and store it in the segment structure
    int n = recv(sockfd, s.header, 20, 0);
    s->header[n] = '\0';
    n = recv(sockfd, s->pseudoheader, 12, 0);
    s->pseudoheader[n] = '\0';
    n = recv(sockfd, s->l3info.SourceIpv4, 16, 0);
    s->l3info.SourceIpv4[n] = '\0';
    n = recv(sockfd, s->l3info.DesIpv4, 16, 0);
    s->l3info.DesIpv4[n] = '\0';
    n = recv(sockfd, &s->l3info.protocol, 4, 0);
    s->l3info.protocol = ntohl(s->l3info.protocol);
    n = recv(sockfd, &s->l4info.AckNum, 4, 0);
    s->l4info.AckNum = ntohl(s->l4info.AckNum);
    n = recv(sockfd, &s->l4info.SeqNum, 4, 0);
    s->l4info.SeqNum = ntohl(s->l4info.SeqNum);
    n = recv(sockfd, &s->l4info.SourcePort, 4, 0);
    s->l4info.SourcePort = ntohl(s->l4info.SourcePort);
    n = recv(sockfd, &s->l4info.DesPort, 4, 0);
    s->l4info.DesPort = ntohl(s->l4info.DesPort);
    n = recv(sockfd, &s->l4info.Flag, 4, 0);
    s->l4info.Flag = ntohl(s->l4info.Flag);
    n = recv(sockfd, &s->l4info.HeaderLen, 4, 0);
    s->l4info.HeaderLen = ntohl(s->l4info.HeaderLen);
    n = recv(sockfd, &s->l4info.WindowSize, 4, 0);
    s->l4info.WindowSize = ntohl(s->l4info.WindowSize);
    printf("%s\n", s->header);
    printf("%s\n", s->pseudoheader);
    printf("%s\n", s->l3info.SourceIpv4);
    printf("%s\n", s->l3info.DesIpv4);
    printf("%d\n", s->l3info.protocol);
    printf("%d\n", s->l4info.AckNum);
    printf("%d\n", s->l4info.SeqNum);
    printf("%d\n", s->l4info.SourcePort);
    printf("%d\n", s->l4info.DesPort);
    printf("%d\n", s->l4info.Flag);
    printf("%d\n", s->l4info.HeaderLen);
    printf("%d\n", s->l4info.WindowSize);
    printf("\n");
}

char* myheadercreater(Segment* s){
    //Creates the header for the segment
    char* header = (char*)malloc(20);
    strcpy(header, s->header);
    strcpy(header + 20, s->pseudoheader);
    strcpy(header + 40, s->l3info.SourceIpv4);
    strcpy(header + 60, s->l3info.DesIpv4);
    strcpy(header + 80, &s->l3info.protocol);
    strcpy(header + 100, &s->l4info.AckNum);
    strcpy(header + 120, &s->l4info.SeqNum);
    strcpy(header + 140, &s->l4info.SourcePort);
    strcpy(header + 160, &s->l4info.DesPort);
    strcpy(header + 180, &s->l4info.Flag);
    strcpy(header + 200, &s->l4info.HeaderLen);
    strcpy(header + 220, &s->l4info.WindowSize);
    return header;
}

void sendheader(int sockfd, char* header){
    //send the header string to the server
    int n = send(sockfd, header, strlen(header), 0);
    if (n < 0) {
        perror("send");
        exit(1);
    }
    printf("%s\n", header);
    printf("\n");
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


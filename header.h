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


void receivedata(int sockfd, Segment* s){
    //Receive data from the server and store it in the segment structure
    char header[20];
    char pseudoheader[12];
    L3info l3info;
    L4info l4info;
    memset(header,0,20);
    memset(pseudoheader,0,12);
    memcpy(header,s->header,20);
    memcpy(pseudoheader,s->pseudoheader,12);
    memcpy(&l3info,header,sizeof(L3info));
    memcpy(&l4info,pseudoheader,sizeof(L4info));
    printf("Received data from %s:%d\n",l3info.SourceIpv4,l4info.DesPort);
    printf("Header: %s\n",header);
    printf("Pseudoheader: %s\n",pseudoheader);
    //printf("Data: %s\n",s->data);
    printf("\n");
    if(l4info.Flag==0x01){
        printf("ACK\n");
        l4info.AckNum++;
        l4info.Flag=0x00;
        memcpy(s->pseudoheader,&l4info,sizeof(L4info));
        sendheader(sockfd,s->header);
    }
    else{
        printf("NACK\n");
        l4info.AckNum++;
        l4info.Flag=0x01;
        memcpy(s->pseudoheader,&l4info,sizeof(L4info));
        sendheader(sockfd,s->header);
    }
}

char* myheadercreater(Segment* s){
    //Creates the header for the segment
    char* header=malloc(20);
    char* pseudoheader=malloc(12);
    memset(header,0,20);
    memset(pseudoheader,0,12);
    memcpy(header,&s->l3info,sizeof(L3info));
    memcpy(pseudoheader,&s->l4info,sizeof(L4info));
    return header;
}

void sendheader(int sockfd, char* header){
    //send the header string to the server
    int n;
    n=send(sockfd,header,20,0);
    if(n<0){
        printf("Error sending header\n");
    }
}
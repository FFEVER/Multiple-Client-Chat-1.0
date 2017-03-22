//
//  cmdHandler.cpp
//  C_Project_Client
//
//  Created by Nattaphol on 11/11/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#include "cmdHandler.hpp"

extern int isConnected; /* check if already connected to server */
extern int serv_sockfd; /* server socket file descriptor */
struct USER me; /* user information */

void login(struct USER *me){
    /* check if already connected to server */
    if(isConnected){
        fprintf(stderr, "You are already connected to server at %s:%d\n",SERVERIP,SERVERPORT);
        logFileWrite("Error: You are already connected to a server.\n");
        return;
    }
    serv_sockfd = connectWithServer();
    /* connectWithServer will return -1 if anything failed,
     and return server socket file descriptor if connected successfully */
    if(serv_sockfd < 0){
        fprintf(stderr,"%s, Connection rejected...\n",strerror(errno));
        logFileWrite("Error: Connection rejected.\n");
        return;
    }
    /* set isConnected to True */
    isConnected = 1;
    /* set server socket file descriptor */
    me->sockfd = serv_sockfd;
    /* if Username is "Anonymous" request server to change name to "Anonymous" */
    if(strcmp(me->client_name, "Anonymous")) setalias(me);
    
    printf("Logged in as %s\n", me->client_name);
    logFileWrite("Logged in as <");
    logFileWrite(me->client_name);
    logFileWrite(">\n");
    
    /* create thread for server response receiver handler */
    struct THREADINFO threadinfo;
    pthread_create(&threadinfo.thread_ID, NULL, receiver, (void *)&threadinfo);
    printf("Receiver started [serv_sock:%d]...\n", serv_sockfd);
}

void logout(struct USER *me){
    size_t sent;
    struct PACKET packet;
    
    if(!isConnected){
        fprintf(stderr,"You are not connected...\n");
        logFileWrite("Error: You are not connected to a server.\n");
        return;
    }
    
    memset(&packet, 0, sizeof(struct PACKET));
    strcpy(packet.command, "exit");
    strcpy(packet.client_name, me->client_name);
    
    /* send request to close this connection */
    sent = send(serv_sockfd, (void *)&packet, sizeof(struct PACKET), 0);
    isConnected = 0;
}

void setalias(struct USER *me){
    size_t sent;
    struct PACKET packet;
    
    if(!isConnected){
        fprintf(stderr, "You are not connected...\n");
        logFileWrite("Error: You are not connected to a server.\n");
        return;
    }
    
    memset(&packet, 0, sizeof(struct PACKET));
    strcpy(packet.command, "alias");
    strcpy(packet.client_name, me->client_name);
    
    /* send request to change name */
    sent = send(serv_sockfd,(void *)&packet, sizeof(struct PACKET), 0);
}


void sendToAll(struct USER *me, char *msg){
    size_t sent;
    struct PACKET packet;
    
    if (!isConnected) {
        fprintf(stderr, "You are not connected....\n");
        logFileWrite("Error: You are not connected to a server.\n");
        return;
    }
    
    msg[BUFFSIZE] = 0;
    
    memset(&packet, 0, sizeof(struct PACKET));
    strcpy(packet.command, "send");
    strcpy(packet.client_name, me->client_name);
    strcpy(packet.buff, msg);
    
    /* send request to send to all client */
    sent = send(serv_sockfd, (void *)&packet, sizeof(struct PACKET), 0);
}

void sendToAlias(struct USER *me, char *target, char *msg){
    size_t sent;
    int targetlen;
    struct PACKET packet;
    
    if(target == NULL)
        return;
    
    if(!isConnected) {
        fprintf(stderr, "You are not connected...\n");
        logFileWrite("Error: You are not connected to a server.\n");
        return;
    }
    msg[BUFFSIZE] = 0;
    
    /* targetlen will be used by adding " " & msg to packet buffer
     to match the syntax that server will knows */
    targetlen = strlen(target);
    
    memset(&packet, 0, sizeof(struct PACKET));
    strcpy(packet.command, "whisp");
    strcpy(packet.client_name, me->client_name);
    /* buff = "target" */
    strcpy(packet.buff, target);
    /* buff = "target " */
    strcpy(&packet.buff[targetlen], " ");
    /* buff = "target msg" */
    strcpy(&packet.buff[targetlen+1], msg);
    
    /* send request to whisp */
    sent = send(serv_sockfd, (void *)&packet, sizeof(struct PACKET), 0);
    return;
}

void status(struct USER *me){
    printf("----User Status-----\n");
    printf("Username: %s\n",me->client_name);
    /* check if connected to server or not
     if connect print server information */
    if (isConnected) {
        printf("Status: Connected to server.\n");
        printf("Server IP address: %s\n",SERVERIP);
        printf("Server Port: %d\n",SERVERPORT);
        printf("Server Socket file descriptor: %d\n",serv_sockfd);
    }
    else{
        printf("Status: Not connected to server.\n");
    }
    printf("--------------------\n");
}

char* getTime(){
    time_t rawtime;
    struct tm *info;
    char buffer[TIMESIZE];
    
    time(&rawtime);
    
    info = localtime(&rawtime);
    
    strftime(buffer,TIMESIZE,"%I:%M%p",info);
    return buffer;
}

char* getDate(){
    time_t rawtime;
    struct tm *info;
    char buffer[TIMESIZE];
    
    time(&rawtime);
    
    info = localtime(&rawtime);
    
    strftime(buffer,TIMESIZE,"%x",info);
    return buffer;
}

//
//  serverHandler.cpp
//  C_Project_Client
//
//  Created by Nattaphol on 11/11/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#include "serverHandler.hpp"

int isConnected; /* check if already connected to server */
int serv_sockfd; /* server socket file descriptor */

int connectWithServer(){
    int newfd;
    struct sockaddr_in serv_addr;
    struct hostent *to;
    
    /* generate address */
    if((to = gethostbyname(SERVERIP)) == NULL){
        fprintf(stderr, "%s, gethostmyname() FAILED...\n",strerror(errno));
        logFileWrite("Error: gethostmyname() FAILED.\n");
        return -1;
    }
    
    /* open a socket */
    if((newfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        fprintf(stderr, "%s, Open socket FAILED...\n",strerror(errno));
        logFileWrite("Error: Open socket FAILED.\n");
        return -1;
    }
    
    /* set initial values */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVERPORT);
    serv_addr.sin_addr = *((struct in_addr *)to->h_addr);
    memset(&(serv_addr.sin_zero), 0, 8);
    
    /* try to connect with server */
    if(connect(newfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1){
        fprintf(stderr,"%s, connect() FAILED...\n",strerror(errno));
        logFileWrite("Error: connect() FAILED.\n");
        return -1;
    }
    
    else{
        printf("Connected to server at %s:%d\n", SERVERIP,SERVERPORT);
        logFileWrite("Connected to server at ");
        logFileWrite(SERVERIP);
        logFileWrite("\n");
        return newfd;
    }
}

void *receiver(void *param){
    size_t recvd;
    struct PACKET packet;
    
    while(isConnected){
        recvd = read(serv_sockfd, (void *)&packet, sizeof(struct PACKET));
        if(!recvd){
            fprintf(stderr,"Connection lost from server...\n");
            isConnected = 0;
            close(serv_sockfd);
            break;
        }
        if(recvd > 0){
            /* check if the server response is an error response or normal message */
            if (strcmp(packet.command, "error") == 0){
                printf("<%s>: %s\n",packet.client_name, packet.buff);
                logFileWrite("Error: ");
                logFileWrite(packet.buff);
            }
            else{
                printf("%s [%s]: %s\n", packet.time, packet.client_name, packet.buff);
                logFileWrite("Message receive from <");
                logFileWrite(packet.client_name);
                logFileWrite(">\n");
                
                chatLogFileWrite(packet.date);
                chatLogFileWrite(" ");
                chatLogFileWrite(packet.time);
                chatLogFileWrite(" <");
                chatLogFileWrite(packet.client_name);
                chatLogFileWrite(">: ");
                chatLogFileWrite(packet.buff);
                chatLogFileWrite("\n");
            }
        }
        memset(&packet, 0, sizeof(struct PACKET));
    }
    return NULL;
}

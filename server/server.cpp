//
//  server.cpp
//  C_SE_MyProjectFinal
//
//  Created by Nattaphol on 11/10/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#include <stdio.h> /* standard input/output/error */
#include <stdlib.h> /* malloc() free() exit() atoi() */

#include <pthread.h> /* threads */
#include <string.h> /* string management & memset() */

#include <unistd.h> /* read(), write() and close() socket */
#include <sys/types.h> /* definition of data types using in <netinet/in.h> & <sys/socket.h> */
#include <netinet/in.h> /* struct sockaddr_in ,that stored neccessary information for internet address */
#include <sys/socket.h> /* bind() connect() accept() socket() */
#include <arpa/inet.h> /* inet_addr() */

#include "client_info.hpp"
#include "linkedMan.hpp"
#include "logfileMan.hpp"
#include "adminHandler.hpp"
#include "clientHandler.hpp"

#define IP "127.0.0.1" /* IP address for server */
#define PORTNO 8080 /* initialize port number */
#define BACKLOG 5 /* the number of connections that can be queued. using by listen(sockfd,BACKLOG) */


extern struct CLIENT_LLIST client_list; /* main client_list from linkedMan.cpp */
extern pthread_t commandThread; /* thread variable for commandHandler(void *) */
extern pthread_mutex_t clientlist_mutex; /* thread mutex which prevent deadlock & dataValueError
                                   while thread is using the same client_list */

/* main body */
int main(int argc, char *argv[]){
    int sockfd, cli_sockfd; /* server socket file descriptor & client socket file descriptor */
    socklen_t clilen; /* contains a size of struct sockaddr_in client_addr , use in accept()*/
    struct sockaddr_in serv_addr, client_addr;
    /* internet address information from <netinet/in.h> */
    /* struct sockaddr_in
     {
     short   sin_family; // must be AF_INET
     u_short sin_port;
     struct  in_addr sin_addr;
     char    sin_zero[8]; // Not used, must be zero
     }; */
    
    /* create a new server logfile */
    logFileCreate();
    
    /* initialize value of client_list */
    listInit(&client_list);
    logFileWrite("Initializing value in client_list to NULL...\n");
    
    pthread_mutex_init(&clientlist_mutex, NULL);
    
    
    /* open a socket */
    logFileWrite("Opening socket...\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Open socket FAILED.\n");
    }
    
    /* initialize internet address information */
    logFileWrite("Initailizing internet address information...\n");
    //memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORTNO); /* convert from host byte order to network byte order. */
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    memset(&(serv_addr.sin_zero), 0, 8);
    
    /* bind address with a socket */
    logFileWrite("Binding address with a socket...\n");
    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(sockaddr)) < 0){
        error("Bind socket FAILED.\n");
    }
    
    /* Start listening for connections */
    logFileWrite("Listening for connections...\n");
    if(listen(sockfd, BACKLOG) < 0){
        error("Listening for connections FAILED.\n");
    }
    
    printf("***Initializing server command handler***\n");
    printf("(type help for admin commands manual)\n\n");
    /* creating command handler thread */
    logFileWrite("Creating commandHandler thread...\n");
    if(pthread_create(&commandThread, NULL, commandHandler, NULL) != 0){
        error("Creating commandThread FAILED.\n");
    }
    
    /* Keep accept for incoming socket request */
    printf("***Starting accept for incoming connections***\n");
    while(1){
        clilen = sizeof(client_addr);
        if((cli_sockfd = accept(sockfd, (struct sockaddr*)&client_addr,(socklen_t*)&clilen)) == -1){
            error("Accept incoming connections FAILED.\n");
        }
        else{
            /* check if server is full */
            if(client_list.size == CLIENT_LIMIT){
                fprintf(stderr,"Connection full, request rejected...\n");
                logFileWrite("Connection full, request rejected...\n");
                continue;
            }
            printf("\nConnection requested received...\n");
            logFileWrite("Connection requested received...\n");
            
            /* assign values of a new client */
            struct THREADINFO threadinfo;
            threadinfo.sockfd = cli_sockfd;
            strcpy(threadinfo.cli_name, "Anonymous");
            
            /* creating new clientHandler thread for a new client */
            pthread_mutex_lock(&clientlist_mutex);
            listInsert(&client_list,&threadinfo);
            pthread_mutex_unlock(&clientlist_mutex);
            pthread_create(&threadinfo.thread_ID, NULL, clientHandler, (void *)&threadinfo);
        }
    }
    return 0;
}

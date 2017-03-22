//
//  clientHandler.cpp
//  C_SE_MyProjectFinal
//
//  Created by Nattaphol on 11/10/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#include "clientHandler.hpp"

extern struct CLIENT_LLIST client_list; /* main client_list from linkedMan.cpp */
extern pthread_t commandThread; /* thread variable for commandHandler(void *) */
extern pthread_mutex_t clientlist_mutex; /* thread mutex which prevent deadlock & dataValueError
                                          while thread is using the same client_list */

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

void *clientHandler(void *t_info){
    struct THREADINFO threadinfo;
    struct PACKET packet;
    struct CLIENT_NODE *curr;
    threadinfo = *(struct THREADINFO *)t_info; /* convert void*t_info to struct THREADINFO */
    ssize_t bytes,sent; /* bytes = stored the size of struct PACKET packet recv from client.
                         sent = stored the status of struct PACKET spacket sent to client. */
    while(1){
        /* receive data sent from client into PACKET packet */
        bytes = read(threadinfo.sockfd, (void *)&packet, sizeof(packet));
        logFileWrite("Receiving packet from client <");
        logFileWrite(packet.cli_name);
        logFileWrite(">\n");
        
        /* check if somethings wrong with receving packet */
        if(!bytes){
            fprintf(stderr, "Connection lost from [%d] %s\n",threadinfo.sockfd,packet.cli_name);
            logFileWrite("Connection lost from <");
            logFileWrite(packet.cli_name);
            logFileWrite(">\n");
            pthread_mutex_lock(&clientlist_mutex);
            listDelete(&client_list, &threadinfo);
            pthread_mutex_unlock(&clientlist_mutex);
            break;
        }
        
        /* check if client is on the list, if not end this thread , if yes go on next*/
        for(curr = client_list.head; curr!=NULL; curr = curr->next){
            if (compare(&curr->threadinfo, &threadinfo) == 0) {
                break;
            }
        }
        if(curr == NULL) break;
        
        /* print which client send which command */
        printf("[%d] %s %s %s\n",threadinfo.sockfd,packet.cli_name,packet.command,packet.buff);
        
        /* check for client sent command to decide what to do*/
        if(strcmp(packet.command, "alias") == 0){
            printf("Set client name <%s> to <%s>\n",threadinfo.cli_name,packet.cli_name);
            logFileWrite("Set client name <");
            logFileWrite(threadinfo.cli_name);
            logFileWrite("> to <");
            logFileWrite(packet.cli_name);
            logFileWrite(">\n");
            pthread_mutex_lock(&clientlist_mutex);
            for(curr = client_list.head; curr != NULL; curr = curr->next){
                /* check is it the same sockfd then update cli_name to the new one */
                if(compare(&curr->threadinfo, &threadinfo) == 0){
                    strcpy(curr->threadinfo.cli_name,packet.cli_name);
                    strcpy(threadinfo.cli_name,packet.cli_name);
                    break;
                }
            }
            pthread_mutex_unlock(&clientlist_mutex);
        }
        else if(strcmp(packet.command,"whisp") == 0){
            int i; /* counter for iterating through packet.buff */
            char target[CLIENT_NAME_LEN]; /* name of target */
            bool found = false;
            
            for(i = 0; packet.buff[i] != ' ';i++); /* loop until found spacebar */
            packet.buff[i++] = 0;   /* buff = "name text" -> buff = "name\0text" */
            strcpy(target, packet.buff);
            
            pthread_mutex_lock(&clientlist_mutex);
            for(curr = client_list.head; curr != NULL; curr = curr->next){
                /* check if it is target or not */
                if(strcmp(target, curr->threadinfo.cli_name) == 0){
                    found = true;
                    /* initialize packet to be sent */
                    struct PACKET spacket;
                    strcpy(spacket.command,"msg");
                    strcpy(spacket.cli_name,threadinfo.cli_name);
                    strcpy(spacket.buff,&packet.buff[i]);
                    strcpy(spacket.time, getTime());
                    strcpy(spacket.date, getDate());
                    /* send spacket to target */
                    sent = send(curr->threadinfo.sockfd, (void *)&spacket, sizeof(struct PACKET), 0);
                    logFileWrite("Sending spacket to <");
                    logFileWrite(target);
                    logFileWrite(">.\n");
                    if(send < 0){
                        fprintf(stderr, "%s, Sent spacket to %s FAILED.\n",strerror(errno),target);
                        logFileWrite("Error: Sent spacket to <");
                        logFileWrite(packet.cli_name);
                        logFileWrite("> FAILED.\n");
                    }
                    //break;
                }
            }
            /* if can't find any the same target name, send error report to client */
            //if(curr == NULL){
            if(found == false){
                struct PACKET spacket;
                strcpy(spacket.command, "error");
                strcpy(spacket.cli_name, "server");
                strcpy(spacket.buff, "Error: Can't find target.");
                logFileWrite("Error: Can't find <");
                logFileWrite(target);
                logFileWrite(">\n");
                /* send error report to client */
                sent = send(threadinfo.sockfd,(void *)&spacket, sizeof(struct PACKET), 0);
                /* check if sent failed */
                if(sent < 0){
                    fprintf(stderr, "%s, Sent spacket(err response) to %s FAILED.\n",strerror(errno),threadinfo.cli_name);
                    logFileWrite("Error: Sent spacket(err response) to <");
                    logFileWrite(threadinfo.cli_name);
                    logFileWrite("> FAILED.\n");
                }
            }
            pthread_mutex_unlock(&clientlist_mutex);
        }
        else if(strcmp(packet.command, "send") == 0){
            pthread_mutex_lock(&clientlist_mutex);
            /* initialize packet to be sent */
            struct PACKET spacket;
            strcpy(spacket.command,"msg");
            strcpy(spacket.cli_name,packet.cli_name);
            strcpy(spacket.buff,packet.buff);
            strcpy(spacket.time, getTime());
            strcpy(spacket.date, getDate());
            /* loop through client_list */
            for(curr = client_list.head; curr != NULL; curr = curr->next){
                if(compare(&curr->threadinfo,&threadinfo) == 0) continue;
                /* send spacket to all client in client_list */
                sent = send(curr->threadinfo.sockfd,(void *)&spacket,sizeof(struct PACKET),0);
                /* check if send failed */
                if(send < 0){
                    fprintf(stderr, "%s, Sent spacket to %s FAILED.\n",strerror(errno),curr->threadinfo.cli_name);
                    logFileWrite("Error: Sent spacket to <");
                    logFileWrite(curr->threadinfo.cli_name);
                    logFileWrite("> FAILED.\n");
                }
            }
            memset(&spacket, 0, sizeof(struct PACKET));
            pthread_mutex_unlock(&clientlist_mutex);
        }
        else if(strcmp(packet.command, "exit") == 0){
            printf("[%d] %s has disconnected...\n", threadinfo.sockfd,threadinfo.cli_name);
            logFileWrite("<");
            logFileWrite(threadinfo.cli_name);
            logFileWrite("> has disconnected...\n");
            /* delete client from the list */
            pthread_mutex_lock(&clientlist_mutex);
            listDelete(&client_list, &threadinfo);
            pthread_mutex_unlock(&clientlist_mutex);
            break;
        }
        else{
            /* check for the garbage data that leftover in the packet */
            fprintf(stderr,"Garbage data from [%d] %s\n",threadinfo.sockfd,packet.cli_name);
            logFileWrite("Garbage data from <");
            logFileWrite(packet.cli_name);
            logFileWrite(">\n");
        }
        
    }
    close(threadinfo.sockfd); /* close client socket */
    return NULL;
}


//
//  adminHandler.cpp
//  C_SE_MyProjectFinal
//
//  Created by Nattaphol on 11/10/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#include "adminHandler.hpp"

extern struct CLIENT_LLIST client_list; /* main client_list from linkedMan.cpp */

pthread_t commandThread; /* thread variable for commandHandler(void *) */
pthread_mutex_t clientlist_mutex; /* thread mutex which prevent deadlock & dataValueError
                                   while thread is using the same client_list */

void *commandHandler(void *null){
    char command[CMDLEN]; /* command help, list, kick, exit */
    while(gets(command),1){
        if(strcmp(command,"exit") == 0){
            logFileWrite("Command -> exit\n");
            printf("Terminating server...\n");
            exit(0); /* terminates program */
        }
        else if(strcmp(command,"help") == 0){
            FILE *help;
            char c;
            logFileWrite("Command -> help\n");
            /* opening help_me.txt */
            help = fopen("help_me.txt","r");
            if(help == 0){
                fprintf(stderr,"%s Can't open file help_me.txt",strerror(errno));
                continue;
            }
            /* print help_me.txt */
            while(fscanf(help, "%c",&c) != EOF){
                printf("%c",c);
            }
            printf("\n");
            fclose(help);
        }
        else if(strcmp(command, "list") == 0){
            logFileWrite("Command -> list\n");
            pthread_mutex_lock(&clientlist_mutex); /* mutex lock */
            listShow(&client_list); /* show all connected clients */
            pthread_mutex_unlock(&clientlist_mutex); /* mutex lock */
        }
        else if(strncmp(command, "kick", strlen("kick")) == 0){
            int status = 0; /* status returned from kickClient() */
            char *ptr = strtok(command, " "); /* token command string to find name of client */
            char name[CLIENT_NAME_LEN]; /* client name that'll be kicked */
            
            ptr = strtok(NULL," "); /* get client name , NULL if can't find */
            logFileWrite("Command -> kick <");
            logFileWrite(ptr);
            logFileWrite(">\n");
            memset(name, 0, strlen(name));
            if (ptr != NULL) {
                strcpy(name, ptr);
                pthread_mutex_lock(&clientlist_mutex); /* mutex lock */
                status = kickClient(&client_list,name);
                if(status == 0){ /* kick successful */
                    printf("%s has been kicked.\n",name);
                }
                else if(status == -1){ /* can't find name */
                    printf("Error: Cannot recognize client name.\n");
                    logFileWrite("Error: Cannot recognize client name.\n");
                }
                else if(status == 1){ /* delete failed */
                    printf("Error: Cannot delete client from list.\n");
                    logFileWrite("Error: Cannot delete client from list.\n");
                }
                pthread_mutex_unlock(&clientlist_mutex); /* mutex unlock */
            }
            else{ /* no client name given */
                printf("Error: No client name given.\n");
                logFileWrite("Error: No client name given.\n");
            }
            /* clear name and ptr */
            memset(name, 0, sizeof(name));
            ptr = NULL;
        }
        else{
            /* if command match none of those */
            printf("Error: Unknown command\n");
            logFileWrite("Error: Unknown command\n");
        }
    }
    
    return NULL;
}

int kickClient(struct CLIENT_LLIST *ll,char cli_name[CLIENT_NAME_LEN]){
    struct THREADINFO *t_info;
    struct CLIENT_NODE *curr;
    bool found = false;
    size_t sent;
    
    if(ll->head == NULL){
        return -1;
    }
    
    for(curr = ll->head; curr != NULL; curr = curr->next){ /* loop through ll until ll->next == NULL */
        t_info = &curr->threadinfo;
        if(strcmp(t_info->cli_name, cli_name) == 0){ /* compare cli_name & t_info->cli_name */
            found = true; /* found client */
            if(listDelete(ll, t_info)< 0)
                return 1; /* delete failed */
            else{
                struct PACKET spacket;
                strcpy(spacket.command, "error");
                strcpy(spacket.cli_name, "server");
                strcpy(spacket.buff, "You has been kicked from server.");
                sent = send(t_info->sockfd, (void *)&spacket, sizeof(struct PACKET), 0);
                if(send < 0){
                    fprintf(stderr, "%s, Sent spacket to %s FAILED.\n",strerror(errno),t_info->cli_name);
                    logFileWrite("Error: Sent spacket to <");
                    logFileWrite(t_info->cli_name);
                    logFileWrite("> FAILED.\n");
                }
                close(t_info->sockfd);
            }
        }
    }
    if(found) return 0; /* delete successful */
    else return -1; /* can't find any cli_name in CLIENT_LLIST */
}


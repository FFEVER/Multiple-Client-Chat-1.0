//
//  client.cpp
//  C_SE_MyProject
//
//  Created by Nattaphol on 10/29/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#include <stdio.h> /* standard input/output/error */
#include <string.h> /* string management, strerror() & memset() */
#include <errno.h> /* errno */

#include "user_info.hpp"
#include "fileMan.hpp"
#include "cmdHandler.hpp"
#include "serverHandler.hpp"

#define LINEBUFF 2048 /* command buffer size */

extern struct USER me; /* user information */
extern int isConnected; /* check if already connected to server */
char command[LINEBUFF]; /* command string input */

/* main body */
int main(int argc, char **argv){
    size_t aliaslen; /* client_name length to check if it larger than CLIENT_NAME_LEN or not */
    
    /* clear struct USER me values before using */
    memset(&me, 0, sizeof(struct USER));
    
    /* create a new client log file */
    logFileCreate();
    
    /* crate a new chat log file */
    chatLogFileCreate();
    
    printf("--------Welcome!---------\n");
    printf("(type help for user commands manual)\n");
    
    /* Keep get command input from user until hit "exit" */
    while(gets(command)){
        if(strncmp(command,"exit",4) == 0){
            logFileWrite("Command -> exit\n");
            /* if connecting with server, logout first */
            if (isConnected) {
                logout(&me);
            }
            /* then break out of the loop */
            break;
        }
        else if(strncmp(command,"help",4) == 0){
            /* open help file named help_me_client.txt */
            char c;
            FILE *helpFile = fopen("help_me_client.txt","r");
            logFileWrite("Command -> help\n");
            /* check for opening file error */
            if(helpFile == 0){
                fprintf(stderr,"%s, Help file not found...\n",strerror(errno));
                logFileWrite("Error: help_me_client.txt not found.\n");
            }
            /* if not error read the whole file */
            while(fscanf(helpFile, "%c",&c) != EOF){
                printf("%c",c);
            }
            printf("\n");
        }
        else if(strncmp(command,"login",5) == 0){
            char *name = strtok(command, " ");
            name = strtok(NULL, " "); /* get Username */
            memset(me.client_name, 0, sizeof(me.client_name)); /* reset values in me.client_name before using */
            
            logFileWrite("Command -> login\n");
            
            /* if Username was given, set me.client_name to name */
            if (name != NULL) {
                aliaslen = strlen(name);
                /* check if given name length exceed the limit or not */
                if (aliaslen > CLIENT_NAME_LEN) name[CLIENT_NAME_LEN] = 0;
                strcpy(me.client_name, name);
            }
            /* if Username wasn't given, set me.client_name to Anonymous */
            else{
                strcpy(me.client_name, "Anonymous");
            }
            /* then login to server */
            login(&me);
        }
        else if(strncmp(command, "alias", 5) == 0){
            char *name = strtok(command, " ");
            name = strtok(NULL, " "); /* get a new Username */
            char oldname[CLIENT_NAME_LEN]; /* stored an old name for writing into logfile */
            strcpy(oldname,me.client_name);
            
            logFileWrite("Command -> alias ");
            if (name != NULL) {
                memset(me.client_name, 0, sizeof(me.client_name)); /* reset values in me.client_name before using */
                
                logFileWrite(name);
                logFileWrite("\n");
                logFileWrite("Username has been changed from <");
                logFileWrite(oldname);
                logFileWrite("> to <");
                logFileWrite(name);
                logFileWrite(">\n");
                
                aliaslen = strlen(name);
                /* check if given name length exceed the limit or not */
                if(aliaslen > CLIENT_NAME_LEN) name[CLIENT_NAME_LEN] = 0;
                strcpy(me.client_name, name);
                /* set a new Username */
                setalias(&me);
            }
            else
                logFileWrite("\n");
        }
        else if(strncmp(command, "whisp", 5) == 0){
            char *msg = strtok(command, " ");
            char cli_name[CLIENT_NAME_LEN];
            msg = strtok(NULL, " "); /* get a target name */
            memset(cli_name, 0, sizeof(cli_name));
            
            logFileWrite("Command -> whisp ");
            /* check if target is given or not */
            if (msg != NULL) {
                aliaslen = strlen(msg);
                /* check if given name length exceed the limit or not */
                if(aliaslen > CLIENT_NAME_LEN) msg[CLIENT_NAME_LEN] = 0;
                strcpy(cli_name, msg); /* copy target name into cli_name */
                logFileWrite("<");
                logFileWrite(cli_name);
                logFileWrite("> ");
                
                /* strtok edit command from "whisp target_name message" to "whisp\0target_name\0message"
                 now, msg pointer is on the character 't', so we loop through it to the last \0. */
                while (*msg) msg++; msg++;
                
                logFileWrite("\"");
                logFileWrite(msg);
                
                logFileWrite("\"\n");
                
                chatLogFileWrite(getDate());
                chatLogFileWrite(" ");
                chatLogFileWrite(getTime());
                chatLogFileWrite(" (whisp ");
                chatLogFileWrite(cli_name);
                chatLogFileWrite(") you: \"");
                chatLogFileWrite(msg);
                chatLogFileWrite("\"\n");
                
                /* send message to target */
                sendToAlias(&me, cli_name, msg);
            }
            /* if not given print error */
            else{
                fprintf(stderr, "Error: Invalid whisp syntax. No client name given.\n");
                logFileWrite("\nError: Invalid whisp syntax. No client name given.\n");
            }
            msg = NULL;
        }
        else if(strncmp(command, "send" , 4) == 0){
            /* send message to all user that connected to client */
            sendToAll(&me, &command[5]);
            
            logFileWrite("Command -> send \"");
            logFileWrite(&command[5]);
            logFileWrite("\"\n");
            
            chatLogFileWrite(getDate());
            chatLogFileWrite(" ");
            chatLogFileWrite(getTime());
            chatLogFileWrite(" you: \"");
            chatLogFileWrite(&command[5]);
            chatLogFileWrite("\"\n");
            
        }
        else if(strncmp(command, "status", 6) == 0){
            /* display status and information of user */
            status(&me);
            logFileWrite("Command -> status\n");
        }
        else if(strncmp(command, "logout", 6) == 0){
            /* logout from server but not exit */
            logout(&me);
            logFileWrite("Command -> logout\n");
        }
        else {
            /* if command match none of those */
            fprintf(stderr, "Unknown command...\n");
            logFileWrite("Error: Unknown command.\n");
        }
        memset(command, 0, LINEBUFF);
    }
    return 0;
}

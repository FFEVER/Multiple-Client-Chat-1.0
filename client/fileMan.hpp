//
//  fileMan.hpp
//  C_SE_MyProjectFinal
//
//  Created by Nattaphol on 11/10/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#ifndef fileMan_hpp
#define fileMan_hpp

#include <stdio.h> /* standard input/output/error */
#include <string.h> /* string management & memset() */

#include <errno.h>


void logFileCreate(void); /* create a new client_log.txt file */
void logFileWrite(const char *msg); /* update a client_log.txt file */
void chatLogFileCreate(void); /* create a new chat log file */
void chatLogFileWrite(const char *msg); /* update a chat log file */

#endif /* fileMan_hpp */

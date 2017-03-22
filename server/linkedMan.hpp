//
//  linkedMan.hpp
//  C_SE_MyProjectFinal
//
//  Created by Nattaphol on 11/10/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#ifndef linkedMan_hpp
#define linkedMan_hpp

#include "client_info.hpp"
#include <stdlib.h>
#include <stdio.h>

#define CLIENT_LIMIT 10 /* limit of a number of client. */

struct CLIENT_NODE{
    struct THREADINFO threadinfo;
    struct CLIENT_NODE *next; /* next node */
};

struct CLIENT_LLIST{
    int size; /* size of client_list */
    struct CLIENT_NODE *head, *tail; /* head & tail pointer to CLIENT_NODE*/
};

int compare(struct THREADINFO *a, struct THREADINFO *b);

void listInit(struct CLIENT_LLIST *ll);

int listInsert(struct CLIENT_LLIST *ll, struct THREADINFO *t_info);

int listDelete(struct CLIENT_LLIST *ll, struct THREADINFO *t_info);

void listShow(struct CLIENT_LLIST *ll);

#endif /* linkedMan_hpp */

//
//  linkedMan.cpp
//  C_SE_MyProjectFinal
//
//  Created by Nattaphol on 11/10/16.
//  Copyright © 2016 Nattaphol. All rights reserved.
//

#include "linkedMan.hpp"

struct CLIENT_LLIST client_list; /* the main client_list */

int compare(struct THREADINFO *a, struct THREADINFO *b){
    /* if sockets are equal each other return 0 */
    return !(a->sockfd == b->sockfd);
}

void listInit(struct CLIENT_LLIST *ll){
    /* before using, we initialize value of client list to be NULL*/
    ll->head = ll->tail = NULL;
    ll->size = 0;
}

int listInsert(struct CLIENT_LLIST *ll, struct THREADINFO *t_info){
    /* check if it is the limit of size of client */
    if(ll->size == CLIENT_LIMIT) return -1;
    /* check if there's no elements in ll or not(means this is the first one to be added) */
    if(ll->head == NULL){
        ll->head = (struct CLIENT_NODE *)malloc(sizeof(struct CLIENT_NODE));
        ll->head->threadinfo = *t_info;
        ll->head->next = NULL;
        ll->tail = ll->head;
    }
    else{
        ll->tail->next = (struct CLIENT_NODE *)malloc(sizeof(struct CLIENT_NODE));
        /* move tail to the next empty node */
        ll->tail = ll->tail->next;
        ll->tail->threadinfo = *t_info;
        /* set next node point to NULL */
        ll->tail->next = NULL;
    }
    /* increase the size of client list by 1*/
    ll->size++;
    return 0;
}

int listDelete(struct CLIENT_LLIST *ll, struct THREADINFO *t_info){
    struct CLIENT_NODE *curr;
    struct CLIENT_NODE *temp;
    /* if there's no items in ll return -1 */
    if(ll->head == NULL) return -1;
    /* check if the node to be deleted if head of the client_list */
    if(compare(&ll->head->threadinfo,t_info) == 0){
        temp = ll->head;
        ll->head = ll->head->next;
        /* if there's no more items left in the client_list ,reset head and tail to NULL*/
        if(ll->head == NULL) ll->tail = ll->head;
        /* free the allocated spaces temp */
        free(temp);
        /* decrease the size of client list by 1*/
        ll->size--;
        return 0;
    }
    for(curr = ll->head;curr->next != NULL;curr = curr->next){
        if(compare(&curr->next->threadinfo, t_info) == 0){
            temp = curr->next;
            /* if curr->next is the last node move tail back 1 node to the current node(curr) */
            if(curr->next == ll->tail) ll->tail = curr;
            curr->next = curr->next->next;
            free(temp);
            ll->size--;
            return 0;
        }
    }
    return -1;
}

void listShow(struct CLIENT_LLIST *ll){
    struct CLIENT_NODE *curr;
    printf("-----------------------------------\n");
    printf("Total connection: %d\n",ll->size);
    /* if total connection is 0, end this function */
    printf("-----------------------------------\n");
    if(ll->size == 0)return;
    /* loop through cilent_list */
    for(curr = ll->head; curr!=NULL; curr = curr->next){
        printf("sockfd: %d   client_name: %s\n",curr->threadinfo.sockfd,curr->threadinfo.cli_name);
    }
    printf("-----------------------------------\n");
    return;
}

//
//  main.c
//  storage-allocator
//
//  Created by Vlad Zhavoronkov on 8/11/19.
//  Copyright © 2019 Vlad Zhavoronkov. All rights reserved.
//

#include <stdlib.h>
#include <unistd.h>
#include <malloc/malloc.h>
#include "Header.h"

#define NALLOC 1024

static Header base; /* the base of program memory??? */
static Header *freep = NULL; /* points to first node of free list */

void *mymalloc(unsigned nbytes);
void myfree(void *pointer);

int main(int argc, const char * argv[]) {
    // insert code here...
    
    char *pointer = (char *) mymalloc(4);

    return 0;
}

void *mymalloc(unsigned nbytes) {
    Header *p; /* pointer for return */
    Header *prevp; /* first pointer of free list */
    Header *morecore(unsigned);
    unsigned nunits;
    
    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    if ((prevp = freep) == NULL) { /* free list didn't point to something - BASE and FREEP initialization */
        base.s.ptr = prevp = freep = &base;
        base.s.size = 0;
    }
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {
            if (p->s.size == nunits)
                prevp->s.ptr = p->s.ptr;
            else {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *)(p + 1);
        }
        if (p == freep)
            if ((p = morecore(nunits)) == NULL)
                return NULL;
    }
}

Header *morecore(unsigned nuinits) {
    char *cp;
    Header *up;
    
    if (nuinits < NALLOC)
        nuinits = NALLOC;
    cp = (char *) sbrk(nuinits);
    if (cp == (char *) - 1)
        return NULL;
    up = (Header *) cp;
    up->s.size = nuinits;
    myfree((void *)(up + 1));
    return freep;
}

void myfree(void *pointer) {
    Header *bp, *p;
    
    bp = (Header *)pointer - 1;
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break;
    if ( bp + bp->s.size == p->s.ptr) {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else
        bp->s.ptr = p->s.ptr;
    if (p + p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else
        p->s.ptr = bp;
    freep = p;
}

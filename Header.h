//
//  Header.h
//  storage-allocator
//
//  Created by Vlad Zhavoronkov on 8/11/19.
//  Copyright © 2019 Vlad Zhavoronkov. All rights reserved.
//

#ifndef Header_h
#define Header_h

typedef long Align;

union header {
    struct {
        union header *ptr;
        unsigned size;
    } s;
    Align x;
};

typedef union header Header;

#endif /* Header_h */

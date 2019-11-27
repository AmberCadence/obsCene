#ifndef ALLOCATORS_H_INCLUDED
#define ALLOCATORS_H_INCLUDED

#include "OBC.h"

//typedef unsigned int OBC_ALLOC_META_TYPE;

typedef unsigned long long int OBC_T;

#define OBC_ALLOC_MAX_CONTRACT_TRIES 3
#define OBC_ALLOC_META_TYPE unsigned char
#define OBC_ALLOC_META_CHUNKSIZE 8
#define OBC_ALLOC_META_CHUNKSIZE_BITS (OBC_ALLOC_META_CHUNKSIZE*OBC_ALLOC_META_BITS)
//#define OBC_ALLOCATOR_META_CHUNKSIZE_MOD(position) ((position)&1024))
#define OBC_ALLOC_META_BITS ((sizeof(OBC_ALLOC_META_TYPE))*CHAR_BIT)
#define OBC_ALLOC_META_MASK ((OBC_ALLOC_META_TYPE)(1 << (sizeof(OBC_ALLOC_META_TYPE)*CHAR_BIT-1)))

//meta addressing is N units to reach approximately size of 2^64
//through lookups -- current is 32bit ish of 1k, 4 is ~2^64
//mutate as needed with other above chunk and comparison size optimizations

#define OBC_ALLOC_META_ADDRESSING 4

///set extras to 0 past size_t maximum value
#define OBC_ALLOC_UNITS_0 (OBC_ALLOC_META_CHUNKSIZE_BITS)
#define OBC_ALLOC_UNITS_1 (OBC_ALLOC_META_CHUNKSIZE_BITS*OBC_ALLOC_UNITS_0)
#define OBC_ALLOC_UNITS_2 (OBC_ALLOC_META_CHUNKSIZE_BITS*OBC_ALLOC_UNITS_1)
#define OBC_ALLOC_UNITS_3 (OBC_ALLOC_META_CHUNKSIZE_BITS*OBC_ALLOC_UNITS_2)
#define OBC_ALLOC_UNITS_4 (OBC_ALLOC_META_CHUNKSIZE_BITS*OBC_ALLOC_UNITS_3)
#define OBC_ALLOC_UNITS_5 (OBC_ALLOC_META_CHUNKSIZE_BITS*OBC_ALLOC_UNITS_4)



#define OBC_ALLOC_META_LIMITS \
{\
    OBC_ALLOC_UNITS_0,OBC_ALLOC_UNITS_1 \
    ,OBC_ALLOC_UNITS_2,OBC_ALLOC_UNITS_3 \
    /*,OBC_ALLOC_UNITS_4,OBC_ALLOC_UNITS_5,*/ \
};

/*
typedef OBC_AllocMeta{

}
*/

/*
typedef struct OBC_Allocator{

    size_t (*calloc)(void** self);
    size_t (*malloc)(void** self);
    size_t (*free)(void** self, size_t unit);

}OBC_Allocator;
*/

//size_t OBC_findFirstLeftBit(OBC_ALLOC_META_TYPE *meta);


#endif // ALLOCATORS_H_INCLUDED

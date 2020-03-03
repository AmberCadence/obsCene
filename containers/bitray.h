#ifndef BITRAY_H_INCLUDED
#define BITRAY_H_INCLUDED


#include "OBC.h"
#include "ray.h"
#include "memswap.h"


#define _OBC_BITRAY_PTR_CAST(arrPtr) ((OBC_BitRay *)(arrPtr))
#define _OBC_BITRAY_OFFSET ((size_t)(&((OBC_BitRay *)NULL)->bits.rawData))
#define OBC_TO_BITRAY_PTR(arrPtr) (_OBC_BITRAY_PTR_CAST(((void*)(arrPtr)) - _OBC_BITRAY_OFFSET))

#define OBC_FROM_BITRAY_PTR(arrPtr) ((void**)(((void*)(arrPtr)) + _OBC_BITRAY_OFFSET))
#define OBC_FROM_BITRAY_VAL(listVal) ((void**)(((void*)(&(listVal))) + _OBC_BITRAY_OFFSET))

#define OBC_BITRAY_META_BITS (sizeof(X_X_LONGEST)*CHAR_BIT)

typedef struct OBC_BitRay{

    OBC_Ray bits;

}OBC_BitRay;

typedef OBC_ERROR_ENUM OBC_ValueBit;

void **OBC_newBitRay();
void *OBC_initBitRay(OBC_BitRay *bitray);
void **OBC_BitRayGetAccessPointer(OBC_BitRay *bitray);

void OBC_freeBitRay(void *arr);
void OBC_freeBitRayData(OBC_BitRay *bitray);

OBC_Offset OBC_BitRayFirstLeft0Bit(OBC_BitRay *bray, size_t pos);

OBC_ValueBit OBC_BitRayDoSet(OBC_BitRay *bray, size_t pos, unsigned char bit);
OBC_ValueBit OBC_BitRayDoGet(OBC_BitRay *bray, size_t pos);

unsigned char OBC_BitRayGet(OBC_BitRay *bray, size_t pos);
void OBC_BitRaySet(OBC_BitRay *bray, size_t pos, unsigned char bit);


OBC_ERROR_ENUM OBC_BitRayDoExpand(OBC_BitRay *bray, size_t bitPosNeeded);
OBC_ERROR_ENUM OBC_BitRayExpand(OBC_BitRay *bray);



#endif // BITRAY_H_INCLUDED
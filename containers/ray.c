#include "ray.h"

/*************************************
Initialization
*************************************/

void **OBC_newRay(size_t unitSize){
    return OBC_newRayComplex(0, unitSize);
}

void **OBC_newRayComplex(OBC_Offset initialReserveCount, size_t unitSize){
    OBC_Ray *ray = malloc(sizeof(OBC_Ray));
    if(ray==NULL){
        return NULL;
    }

    if(OBC_initRayDynamic(ray,initialReserveCount, unitSize) == OBC_ERROR_FAILURE){
        free(ray);
        return NULL;
    }

    return OBC_RayGetDataPointer(ray);
}

OBC_ERROR_ENUM OBC_initRay(OBC_Ray *ray, size_t unitSize){
    return OBC_initRayDynamic(ray, 0, unitSize);
}

OBC_ERROR_ENUM OBC_initRayDynamic(OBC_Ray *ray, OBC_Offset initialReserveCount, size_t unitSize){
    return OBC_initRayComplex(ray, NULL, initialReserveCount, unitSize, OBC_TRUE, OBC_TRUE);
}

OBC_ERROR_ENUM OBC_initRayComplex(OBC_Ray *ray, void* bufferStorage, OBC_Offset initialReserveCount, size_t unitSize, OBC_bool canReallocBuffer, OBC_bool canMallocNewBuffer){

    if( OBC_RAY_SIZE_MASK < unitSize
        || (bufferStorage == NULL && !canMallocNewBuffer)
        || (initialReserveCount == 0 && !canMallocNewBuffer) ){
        puts("SANITY CHECKS FAIL");
        return OBC_ERROR_FAILURE;
    }

    if(initialReserveCount==0 || unitSize == 0 || initialReserveCount == 0){
        ray->rawData=NULL;
        ray->maxUnitLength = 0;
    }else if(bufferStorage == NULL){
        ray->rawData = malloc(unitSize*initialReserveCount);
        if(ray->rawData==NULL){
            puts("INIT ALLOCATION CHECKS FAIL");
            return OBC_ERROR_FAILURE;
        }
        ray->maxUnitLength = initialReserveCount;
    }else{
        ray->rawData = (char*)bufferStorage;
        ray->maxUnitLength = initialReserveCount;
    }

    ray->curUnitLength = 0;
    ray->unitSize = unitSize;

    if(canReallocBuffer){
        ray->unitSize ^= OBC_RAY_CAN_REALLOC_MASK;
    }
    if(canMallocNewBuffer){
        ray->unitSize ^= OBC_RAY_CAN_MALLOC_MASK;
    }

    return OBC_ERROR_SUCCESS;

}

/*************************************
Deallocation
*************************************/

void OBC_freeRay(void *rawPtr){
    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    OBC_freeRayData(ray);
    free(ray);
}

void OBC_freeRayData(OBC_Ray *ray){
    if(ray->rawData != NULL && (OBC_RayCanFreeRaw(ray) || OBC_RayCanReallocRaw(ray))){
        free(ray->rawData);
    }
}

void OBC_freeRayNotData(OBC_Ray *ray){
    free(ray);
}

/*************************************
Data Accessors
*************************************/

void **OBC_RayGetDataPointer(OBC_Ray *ray){
    return OBC_FROM_RAY_PTR(ray);
}

char *OBC_RayGetData(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayGetDataRaw(ray);
}

char *OBC_RayGetDataRaw(OBC_Ray *ray){
    return ray->rawData;
}

OBC_Offset OBC_RayGetCurUnitLength(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayGetCurUnitLengthRaw(ray);
}

OBC_Offset OBC_RayGetCurUnitLengthRaw(OBC_Ray *ray){
    return ray->curUnitLength;
}

size_t OBC_RayGetCurByteLength(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayGetCurByteLengthRaw(ray);
}

size_t OBC_RayGetCurByteLengthRaw(OBC_Ray *ray){
    return ((size_t)ray->curUnitLength) * OBC_RayGetUnitSizeRaw(ray);
}

size_t OBC_RayGetMaxByteLength(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayGetMaxByteLengthRaw(ray);
}

size_t OBC_RayGetMaxByteLengthRaw(OBC_Ray *ray){
    return ((size_t)ray->maxUnitLength) * OBC_RayGetUnitSizeRaw(ray);
}

OBC_Offset OBC_RayGetUnitSize(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayGetUnitSizeRaw(ray);
}

OBC_Offset OBC_RayGetUnitSizeRaw(OBC_Ray *ray){
    return ray->unitSize & OBC_RAY_SIZE_MASK;
}

OBC_Offset OBC_RayGetMaxUnitLength(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayGetMaxUnitLengthRaw(ray);
}

OBC_Offset OBC_RayGetMaxUnitLengthRaw(OBC_Ray *ray){
    return ray->maxUnitLength;
}

OBC_bool OBC_RayCanRealloc(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayCanReallocRaw(ray);
}

OBC_bool OBC_RayCanReallocRaw(OBC_Ray *ray){
    return (ray->unitSize & OBC_RAY_CAN_REALLOC_MASK) != 0;
}

OBC_bool OBC_RayCanMalloc(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayCanReallocRaw(ray);
}

OBC_bool OBC_RayCanMallocRaw(OBC_Ray *ray){
    return (ray->unitSize & OBC_RAY_CAN_MALLOC_MASK) != 0;
}

OBC_bool OBC_RayCanFree(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayCanFreeRaw(ray);
}

OBC_bool OBC_RayCanFreeRaw(OBC_Ray *ray){
    return (ray->unitSize & OBC_RAY_CAN_FREE_MASK) != 0;
}

/*************************************
Operations
*************************************/

OBC_ERROR_ENUM OBC_RayRemove(void *rawPtr, OBC_Offset index){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayRemoveRaw(ray, index);
}

OBC_ERROR_ENUM OBC_RayRemoveRaw(OBC_Ray *ray, OBC_Offset index){

    size_t unitSize = OBC_RayGetUnitSizeRaw(ray);
    void *toRemove = ray->rawData+(index*unitSize);

    if(index!=ray->curUnitLength-1){
        memmove(toRemove,toRemove+unitSize,(ray->curUnitLength-(index+1))*unitSize);
    }

    ray->curUnitLength--;

    return OBC_ERROR_NO_OP;
}


OBC_ERROR_ENUM OBC_RayRemoveFast(void *rawPtr, OBC_Offset index){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);

    return OBC_RayRemoveFastRaw(ray, index);
}

OBC_ERROR_ENUM OBC_RayRemoveFastRaw(OBC_Ray *ray, OBC_Offset index){

    size_t unitSize = OBC_RayGetUnitSizeRaw(ray);
    void *toRemove = ray->rawData+(index * unitSize);
    void *toReplace = ray->rawData+((ray->curUnitLength-1) * unitSize);

    if(toReplace > toRemove){
        memmove(toRemove, toReplace, unitSize);
    }

    ray->curUnitLength--;

    return OBC_ERROR_NO_OP;
}


OBC_Offset OBC_RayNewElement(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayNewElementRaw(ray);
}

OBC_Offset OBC_RayNewElementRaw(OBC_Ray *ray){

    OBC_Offset where = ray->curUnitLength;
    if(OBC_RayDoExpandRaw(ray) == OBC_ERROR_FAILURE){
        return OBC_NULL_INDEX;
    }

    OBC_RayPushElementRaw(ray);
    return where;
}

void OBC_RayPushElement(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    OBC_RayPushElementRaw(ray);
}

void OBC_RayPushElementRaw(OBC_Ray *ray){
    ray->curUnitLength++;
}


void OBC_RayPopElement(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    OBC_RayPopElementRaw(ray);
}

void OBC_RayPopElementRaw(OBC_Ray *ray){
    ray->curUnitLength--;
}



OBC_ERROR_ENUM OBC_RayDoExpand(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayDoExpandRaw(ray);
}

OBC_ERROR_ENUM OBC_RayDoExpandRaw(OBC_Ray *ray){

    if(ray->curUnitLength>=ray->maxUnitLength){

        if(OBC_RayExpandRaw(ray) == OBC_ERROR_FAILURE){
            return OBC_ERROR_FAILURE;
        }

        return OBC_ERROR_SUCCESS;
    }

    return OBC_ERROR_NO_OP;
}

OBC_ERROR_ENUM OBC_RayExpand(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayExpandRaw(ray);
}

OBC_ERROR_ENUM OBC_RayExpandRaw(OBC_Ray *ray){

    OBC_Offset units = ray->maxUnitLength;
    if(units*2 < units){
        units = OBC_NULL_INDEX-1;
        if(units == ray->maxUnitLength){
            return OBC_ERROR_NO_OP;
        }
    }else{
        units *= 2;
    }

    size_t unitSize = OBC_RayGetUnitSizeRaw(ray);

    if(unitSize > 0){

        if(!OBC_RayCanMalloc(ray)){
            return OBC_ERROR_NO_OP;
        }

        size_t size = units * unitSize;
        if(size==0){
            size = unitSize;
            units = 1;
        }

        char *newData;
        if(ray->rawData == NULL || (!OBC_RayCanFree(ray) && !OBC_RayCanRealloc(ray))){
            newData = malloc(size);
            if(newData != NULL){
                ray->unitSize ^= OBC_RAY_CAN_FREE_MASK;
            }
        }else{
            newData = realloc(ray->rawData, size);
        }

        if(newData == NULL){
            return OBC_ERROR_FAILURE;
        }

        ray->rawData = newData;

    }

    ray->maxUnitLength = units;

    return OBC_ERROR_SUCCESS;
}

///https://stackoverflow.com/questions/12125308/can-realloc-fail-return-null-when-trimming
OBC_ERROR_ENUM OBC_RayContract(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayContractRaw(ray);
}

OBC_ERROR_ENUM OBC_RayContractRaw(OBC_Ray *ray){

    size_t unitSize = OBC_RayGetUnitSizeRaw(ray);
    size_t newSize;

    if(ray->rawData==NULL || unitSize == 0){
        return OBC_ERROR_NO_OP;
    }

    newSize = ((size_t)ray->maxUnitLength) * (unitSize/2);

    if(newSize < unitSize){
        newSize = unitSize;
    }

    char *newData = NULL;

    if(OBC_RayCanReallocRaw(ray) || OBC_RayCanFreeRaw(ray)){
        newData = realloc(ray->rawData, newSize);
    } else {
        return OBC_ERROR_NO_OP;
    }

    if(newData == NULL){
        return OBC_ERROR_FAILURE;
    }
    ray->rawData = newData;
    ray->maxUnitLength = (OBC_Offset)(newSize/unitSize);
    if(ray->curUnitLength > ray->maxUnitLength){
        ray->curUnitLength = ray->maxUnitLength;
    }

    return OBC_ERROR_SUCCESS;
}


OBC_ERROR_ENUM OBC_RayShrinkToFit(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayShrinkToFitRaw(ray);
}

OBC_ERROR_ENUM OBC_RayShrinkToFitRaw(OBC_Ray *ray){

    if(OBC_RayCanMallocRaw(ray)){
        return OBC_ERROR_NO_OP;
    }

    size_t unitSize = OBC_RayGetUnitSizeRaw(ray);
    size_t newSize = ((size_t)ray->maxUnitLength) * unitSize;//<<1;
    char *newData;

    if(ray->rawData==NULL || unitSize == 0){
        return OBC_ERROR_NO_OP;
    }else{
        newData = realloc(ray->rawData, newSize);
    }

    if(newData==NULL){
        return OBC_ERROR_FAILURE;
    }

    ray->rawData = newData;
    ray->maxUnitLength = (OBC_Offset)(newSize/unitSize);

    return OBC_ERROR_SUCCESS;
}


OBC_ERROR_ENUM OBC_RaySetLast(void *rawPtr, void *data){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RaySetLastRaw(ray, data);
}

OBC_ERROR_ENUM OBC_RaySetLastRaw(OBC_Ray *ray, void *data){

    if(OBC_RayDoExpandRaw(ray) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    size_t unitSize = OBC_RayGetUnitSizeRaw(ray);
    size_t size = unitSize;
    unsigned char *tcpy = (unsigned char *)data;
    unsigned char *unit = (unsigned char *)(ray->rawData+(ray->curUnitLength * unitSize));
    do{
        *unit=*tcpy;
        tcpy++;
        unit++;
    }while(--size);

    return OBC_ERROR_SUCCESS;
}

void* OBC_RayGetLast(void *rawPtr){

    OBC_Ray *ray = OBC_TO_RAY_PTR(rawPtr);
    return OBC_RayGetLastRaw(ray);
}

void* OBC_RayGetLastRaw(OBC_Ray *ray){

    size_t unitSize = OBC_RayGetUnitSizeRaw(ray);
    if(ray->curUnitLength == ray->maxUnitLength){
        return ray->rawData +(((size_t)(ray->curUnitLength-1))*unitSize);
    }
    //unsigned char *unit = (unsigned char *)(ray->rawData+ray->curLength);
    return ray->rawData+(((size_t)ray->curUnitLength)*unitSize);
}



/*************************************
Iteration
*************************************/


void OBC_RayIterStart(void *arr, OBC_RayIterator *iter){

    OBC_Ray *ray = OBC_TO_RAY_PTR(arr);
    OBC_RayIterStartRaw(ray, iter);
}

void OBC_RayIterStartRaw(OBC_Ray *ray, OBC_RayIterator *iter){
    iter->iter = 0;
    iter->endIter = OBC_RayGetCurUnitLengthRaw(ray);
}

void OBC_RayIterNext(void *arr, OBC_RayIterator *iter){

    OBC_Ray *ray = OBC_TO_RAY_PTR(arr);
    OBC_RayIterNextRaw(ray, iter);
}

void OBC_RayIterNextRaw(OBC_Ray *ray, OBC_RayIterator *iter){
    iter->iter++;
}

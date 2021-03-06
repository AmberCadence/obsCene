#include "dequeue.h"

void **OBC_newDequeue(size_t unitSize){

    OBC_Dequeue *dequeue = calloc(1,sizeof(OBC_Dequeue));

    if(dequeue == NULL){
        return NULL;
    }

    if(OBC_initDequeue(dequeue, unitSize) == OBC_ERROR_FAILURE){
        free(dequeue);
        return NULL;
    }

    return OBC_DequeueGetDataPointer(dequeue);

}

OBC_ERROR_ENUM OBC_initDequeue(OBC_Dequeue *dequeue, size_t unitSize){

    if(OBC_initQueue(&dequeue->queue, unitSize) == OBC_ERROR_FAILURE){
        return OBC_ERROR_FAILURE;
    }

    return OBC_ERROR_SUCCESS;
}

void **OBC_DequeueGetDataPointer(OBC_Dequeue *queue){
    return OBC_QueueGetDataPointer(&queue->queue);
}

void OBC_freeDequeue(void *arr){

    OBC_Dequeue *queue = OBC_TO_DEQUEUE_PTR(arr);
    OBC_freeDequeueData(queue);
    free(queue);
}
void OBC_freeDequeueData(OBC_Dequeue *dequeue){

    OBC_freeQueueData(& dequeue->queue);
}

OBC_Offset OBC_DequeuePopFrontRaw(OBC_Dequeue *dequeue){

    if(dequeue->queue.count == 0){
        return dequeue->queue.head;
    }

    dequeue->queue.count--;
    dequeue->queue.head++;
    if(dequeue->queue.head == dequeue->queue.backed.maxUnitLength){
        dequeue->queue.head = 0;
    }

    return dequeue->queue.head;
}

OBC_Offset OBC_DequeuePopBackRaw(OBC_Dequeue *dequeue){
    return OBC_QueuePopRaw(& dequeue->queue);
}

OBC_Offset OBC_DequeuePopFront(void *arr){
    OBC_Dequeue *dequeue = OBC_TO_DEQUEUE_PTR(arr);
    return OBC_DequeuePopFrontRaw(dequeue);
}

OBC_Offset OBC_DequeuePopBack(void *arr){

    OBC_Dequeue *dequeue = OBC_TO_DEQUEUE_PTR(arr);
    return OBC_DequeuePopBackRaw(dequeue);
}

OBC_Offset OBC_DequeuePushFrontRaw(OBC_Dequeue *dequeue){

    if(dequeue->queue.count >= dequeue->queue.backed.maxUnitLength){
        if(OBC_QueueExpandRaw(& dequeue->queue) == OBC_ERROR_FAILURE){
            return OBC_NULL_INDEX;
        }
    }

    dequeue->queue.count++;
    if(dequeue->queue.head == 0){
        dequeue->queue.head = dequeue->queue.backed.maxUnitLength-1;
    }else{
        dequeue->queue.head--;
    }

    return dequeue->queue.head;
}

OBC_Offset OBC_DequeuePushBackRaw(OBC_Dequeue *dequeue){
    return OBC_QueuePushRaw(& dequeue->queue);
}

OBC_Offset OBC_DequeuePushFront(void *arr){

    OBC_Dequeue *dequeue = OBC_TO_DEQUEUE_PTR(arr);
    return OBC_DequeuePushFrontRaw(dequeue);
}

OBC_Offset OBC_DequeuePushBack(void *arr){

    OBC_Dequeue *dequeue = OBC_TO_DEQUEUE_PTR(arr);
    return OBC_DequeuePushBackRaw(dequeue);
}


OBC_ERROR_ENUM OBC_DequeueAddFrontRaw(OBC_Dequeue *dequeue, void* item){

    OBC_Offset pos = OBC_DequeuePushFrontRaw(dequeue);
    if(pos == OBC_NULL_INDEX){
        return OBC_ERROR_FAILURE;
    }

    return OBC_DequeueAddRaw(dequeue, pos, item);
}

OBC_ERROR_ENUM OBC_DequeueAddBackRaw(OBC_Dequeue *dequeue, void* item){

    OBC_Offset pos = OBC_DequeuePushBackRaw(dequeue);
    if(pos == OBC_NULL_INDEX){
        return OBC_ERROR_FAILURE;
    }

    return OBC_DequeueAddRaw(dequeue,pos,item);
}

OBC_ERROR_ENUM OBC_DequeueAddFront(void *arr, void* item){

    OBC_Dequeue *dequeue = OBC_TO_DEQUEUE_PTR(arr);
    return OBC_DequeueAddFrontRaw(dequeue, item);
}

OBC_ERROR_ENUM OBC_DequeueAddBack(void *arr, void* item){

    OBC_Dequeue *dequeue = OBC_TO_DEQUEUE_PTR(arr);
    return OBC_DequeueAddBackRaw(dequeue, item);
}

OBC_ERROR_ENUM OBC_DequeueAdd(void *arr, OBC_Offset pos, void* item){

    OBC_Dequeue *dequeue = OBC_TO_DEQUEUE_PTR(arr);
    return OBC_DequeueAddRaw(dequeue, pos, item);
}
OBC_ERROR_ENUM OBC_DequeueAddRaw(OBC_Dequeue *dequeue, OBC_Offset pos, void* item){

    if(pos == OBC_NULL_INDEX){
        return OBC_ERROR_FAILURE;
    }

    size_t unitSize = OBC_RayGetUnitSizeRaw(&dequeue->queue.backed);
    memcpy(dequeue->queue.backed.rawData + (unitSize * pos), item, unitSize);
    //OBC_X_MEMCPY(dequeue->queue.backed.rawData, unitSize,pos,item);

    return OBC_ERROR_SUCCESS;
}




OBC_Offset OBC_DequeueIterStartRaw(OBC_Dequeue *dequeue){
    return OBC_QueueIterStartRaw(& dequeue->queue);
}
OBC_Offset OBC_DequeueIterStart(void *arr){

    OBC_Dequeue *dequeue = OBC_TO_DEQUEUE_PTR(arr);
    return OBC_DequeueIterStartRaw(dequeue);
}

OBC_Offset OBC_DequeueIterNextRaw(OBC_Dequeue *dequeue, OBC_Offset iter){
    return OBC_QueueIterNextRaw(& dequeue->queue,iter);
}
OBC_Offset OBC_DequeueIterNext(void *arr, OBC_Offset iter){

    OBC_Dequeue *dequeue = OBC_TO_DEQUEUE_PTR(arr);
    return OBC_DequeueIterNextRaw(dequeue, iter);
}

#include "dequeue.h"

void **OBC_newDequeue(size_t unitSize){

    OBC_Dequeue *dequeue = calloc(1,sizeof(OBC_Dequeue));

    if(OBC_initDequeue(dequeue,unitSize) == NULL){
        free(dequeue);
        return NULL;
    }

    return (void **) &dequeue->queue.backed.rawData;

}
void *OBC_initDequeue(OBC_Dequeue *dequeue, size_t unitSize){

    if(OBC_initQueue(& dequeue->queue,unitSize) == NULL){
        return NULL;
    }

    return dequeue;
}

void OBC_freeDequeue(void *arr){
    OBC_Dequeue *queue = OBC_TO_DEQUEUE_PTR(arr);
    OBC_freeDequeueData(queue);
    free(queue);
}
void OBC_freeDequeueData(OBC_Dequeue *dequeue){
    OBC_freeQueueData(& dequeue->queue);
}

OBC_Offset OBC_DequeuePopFrontRaw(OBC_Dequeue *dequeue);
OBC_Offset OBC_DequeuePopBackRaw(OBC_Dequeue *dequeue);
OBC_Offset OBC_DequeuePopFront(void *arr);
OBC_Offset OBC_DequeuePopBack(void *arr);

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
    return OBC_DequeuePushFrontRaw(OBC_TO_DEQUEUE_PTR(arr));
}
OBC_Offset OBC_DequeuePushBack(void *arr){
    return OBC_DequeuePushBackRaw(OBC_TO_DEQUEUE_PTR(arr));
}

OBC_ERROR_ENUM OBC_DequeueAddFrontRaw(OBC_Dequeue *dequeue, void* item){
    size_t pos = OBC_DequeuePushFrontRaw(dequeue);
    if(pos == OBC_NULL_INDEX){
        return OBC_ERROR_FAILURE;
    }
    return OBC_DequeueAddRaw(dequeue,pos,item);
}
OBC_ERROR_ENUM OBC_DequeueAddBackRaw(OBC_Dequeue *dequeue, void* item){
    size_t pos = OBC_DequeuePushBackRaw(dequeue);
    if(pos == OBC_NULL_INDEX){
        return OBC_ERROR_FAILURE;
    }
    return OBC_DequeueAddRaw(dequeue,pos,item);
}

OBC_ERROR_ENUM OBC_DequeueAddFront(void *arr, void* item){
    return OBC_DequeueAddFrontRaw(OBC_TO_DEQUEUE_PTR(arr),item);
}
OBC_ERROR_ENUM OBC_DequeueAddBack(void *arr, void* item){
    return OBC_DequeueAddBackRaw(OBC_TO_DEQUEUE_PTR(arr),item);
}

OBC_ERROR_ENUM OBC_DequeueAdd(void *arr, OBC_Offset pos, void* item){
    return OBC_DequeueAddRaw(OBC_TO_DEQUEUE_PTR(arr),pos,item);
}
OBC_ERROR_ENUM OBC_DequeueAddRaw(OBC_Dequeue *dequeue, OBC_Offset pos, void* item){
    if(pos == OBC_NULL_INDEX){
        return OBC_ERROR_FAILURE;
    }

    OBC_X_MEMCPY(dequeue->queue.backed.rawData,
                 dequeue->queue.backed.unitSize,pos,item);

    return OBC_ERROR_SUCCESS;
}




OBC_Offset OBC_DequeueIterStartRaw(OBC_Dequeue *dequeue){
    return OBC_QueueIterStartRaw(& dequeue->queue);
}
OBC_Offset OBC_DequeueIterStart(void *arr){
    return OBC_DequeueIterStartRaw(OBC_TO_DEQUEUE_PTR(arr));
}

OBC_Offset OBC_DequeueIterNextRaw(OBC_Dequeue *dequeue, OBC_Offset iter){
    return OBC_QueueIterNextRaw(& dequeue->queue,iter);
}
OBC_Offset OBC_DequeueIterNext(void *arr, OBC_Offset iter){
    return OBC_DequeueIterNextRaw(OBC_TO_DEQUEUE_PTR(arr),iter);
}



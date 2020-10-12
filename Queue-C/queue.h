/**
 * @file    queue.h
 * @brief   Create by AnKun on 2019/10/30
 * @version 1.0.0
 */
#ifndef __QUEUE_H_
#define __QUEUE_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/// ���п��ƽṹ��
typedef struct
{
    uint32_t read;			///< ��ָ��
    uint32_t write;			///< дָ��
    uint32_t size;			///< ���д�С(�ֽ�)
    uint8_t* payload;		///< ���и�������
}Queue, *QueuePtr;

/// �������ֽڲ�Ԥ�����ռ�
#define QALIGN4(size)  (((size + 5) >> 2) << 2)

/// ��������
uint32_t Queue_GetSize(const QueuePtr queue);
uint32_t Queue_GetUsed(const QueuePtr queue);
uint32_t Queue_GetFree(const QueuePtr queue);
uint32_t Queue_Write(QueuePtr queue, const void* buf, uint32_t length);
uint32_t Queue_Read(QueuePtr queue, void* buf, uint32_t length);
void Queue_Clear(QueuePtr queue);
void Queue_ErrBack(QueuePtr queue, uint32_t len);
void Queue_Init(QueuePtr queue, uint32_t size, void* payload);
void Queue_Destory(QueuePtr queue);
int Queue_isFull(const QueuePtr queue);
int Queue_isEmpty(const QueuePtr queue);
QueuePtr Queue_Create(uint32_t size);

#endif // __QUEUE_H_





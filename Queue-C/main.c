/*
 * mian.c
 *
 *  Created on: 2020��10��12��
 *      Author: hello
 */

#include <stdio.h>
#include <string.h>
#include "queue.h"

/* ���д�С */
#define QUEUE_SIZE 100

/* �������ݿռ䡣��ʹ�ú�QALIGN4��Ŀ����Ϊ�˸��ݶ��д�С����ʵ����Ҫ�Ķ��д洢�ռ��С������4�ֽ� */
uint8_t QueueBuffer[QALIGN4(QUEUE_SIZE)];

/* ���о�� */
Queue queue;
QueuePtr queueHandle = &queue;

int main()
{
	char buf[20];

	memset(buf, 0X00, sizeof(buf));

	// ��̬��ʼ��
	Queue_Init(&queue, QUEUE_SIZE, QueueBuffer);

	// д
	Queue_Write(queueHandle, "HelloWorld", 10);

	// ��
	Queue_Read(queueHandle, buf, 10);

	// ��ӡ
	printf("read:%s\n", buf);

	return 0;
}

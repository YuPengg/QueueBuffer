/**
 * @file    queue.c
 * @brief   Create by AnKun on 2019/10/30
 * @version 1.0.0
 */
#include "queue.h"

static void qmemcpy(void* dst, const void* src, uint32_t len)
{
	char *d = (char *) dst;
	const char *s = (const char *) src;

	while (len >= sizeof(int))
	{
		*(int*) d = *(int*) s;
		d += sizeof(int);
		s += sizeof(int);
		len -= sizeof(int);
	}
	while ((len) > 0U)
	{
		*d = *s;
		d += 1U;
		s += 1U;
		len -= 1U;
	}
}

/**
 * ��ö���ʹ����
 * @param  queue ���о��
 * @return       ������ʹ����
 */
uint32_t Queue_GetUsed(const QueuePtr queue)
{
	int len = queue->write - queue->read;
	return len >= 0 ? len : (queue->size + len);
}

/**
 * ��ö��п���ռ�
 * @param  queue ���о��
 * @return       ���п�����
 */
uint32_t Queue_GetFree(const QueuePtr queue)
{
	int len = queue->read - queue->write;
	return len > 0 ? (queue->size - len) : (queue->size - 1 + len);
}

/**
 * ��ö��д�С
 * @param  queue ���о��
 * @return       ���д�С
 */
uint32_t Queue_GetSize(const QueuePtr queue)
{
	return queue->size - 1;
}

/**
 * �������Ƿ���
 * @param  queue
 * @return       1:��    0:����
 */
int Queue_isFull(const QueuePtr queue)
{
	if (queue->read == (queue->write + 1) % queue->size)
		return 1;
	else
		return 0;
}

/**
 * �������Ƿ�Ϊ��
 * @param  queue
 * @return       1:��    fasle:�ǿ�
 */
int Queue_isEmpty(const QueuePtr queue)
{
	if (queue->read == queue->write)
		return 1;
	else
		return 0;
}

/**
 * �������
 * @param  queue  ���о��
 * @param  buf    Ҫ��ӵ�����
 * @param  length Ҫ��ӵ����ݳ���
 * @return        ������ӵ��ֽ���
 */
uint32_t Queue_Write(QueuePtr queue, const void* buf, uint32_t length)
{
	uint8_t *dataptr = (uint8_t *)buf;
	uint32_t offset = 0;
	uint32_t nwrite = 0;
	uint32_t nfree = 0;

	/* ��������ݳ���Ϊ0, ֱ�ӷ��� */
	if (length == 0)		return 0;

	/* ����û�пռ�, ֱ�ӷ��� */
	if ((nfree = Queue_GetFree(queue)) == 0)		return 0;

	/* ����ʵ���ܹ���ӵ����ݳ��� */
	nwrite = nfree >= length ? length : nfree;

	/* �ж϶����Ƿ��β */
	offset = queue->size - queue->write;
	if (offset >= nwrite)
	{
		qmemcpy(queue->payload + queue->write, dataptr, nwrite);
		queue->write += nwrite;
	}
	else
	{
		qmemcpy(queue->payload + queue->write, dataptr, offset);
		qmemcpy(queue->payload, dataptr + offset, nwrite - offset);
		queue->write = nwrite - offset;
	}
	return nwrite;
}

/**
 * ���ݳ���
 * @param  queue  ���о��
 * @param  buf    ��ų��ӵ�����
 * @param  length ���ӵ����ݳ���
 * @return        ���س����ֽ���
 */
uint32_t Queue_Read(QueuePtr queue, void* buf, uint32_t length)
{
	uint8_t *dataptr = (uint8_t *)buf;
	uint32_t offset = 0;
	uint32_t nused = 0;
	uint32_t nread = 0;

	/* �������ݳ���Ϊ0, ֱ�ӷ��� */
	if (length == 0)		return 0;

	/* ����ʵ���ܹ����ӵ����ݳ��� */
	if ((nused = Queue_GetUsed(queue)) == 0)		return 0;

	/* ����ʵ���ܹ����������ݳ��� */
	nread = nused >= length ? length : nused;

	/* �ж�Ҫ���������Ƿ��β */
	offset = queue->size - queue->read;
	if ( offset >= nread)
	{
		qmemcpy(dataptr, queue->payload + queue->read, nread);
		queue->read += nread;
	}
	else
	{
		qmemcpy(dataptr, queue->payload + queue->read, offset);
		qmemcpy(dataptr + offset, queue->payload, nread - offset);
		queue->read = nread - offset;
	}

	return nread;
}

/**
 * ��ʼ��һ������
 * @param queue   ���о��
 * @param size    ���д�С
 * @param payload ���л����ַ
 */
void Queue_Init(QueuePtr queue, uint32_t size, void* payload)
{
	queue->read = 0;
	queue->write = 0;
	queue->payload = (uint8_t *)payload;
	queue->size = size + 1;
}

/**
 * ��ն���
 * @param queue ���о��
 */
void Queue_Clear(QueuePtr queue)
{
	queue->write = queue->read;
}

/**
 * ��̬����һ������
 * @param  size ���д�С
 * @return      �ɹ����ض��ж���ָ��, ʧ�ܷ���NULL
 */
QueuePtr Queue_Create(uint32_t size)
{
	QueuePtr queue = NULL;

	if ((queue = (QueuePtr)malloc(sizeof(Queue))) == NULL)
		return NULL;

	queue->size = size + 1;
	queue->read = 0;
	queue->write = 0;
	queue->payload = NULL;

	if ((queue->payload = (uint8_t *)malloc(QALIGN4(size))) == NULL)
	{
		free(queue);
		return NULL;
	}

	return queue;
}

/**
 * ���ڶ�̬�����Ķ��н���������
 * @param queue ���о��
 */
void Queue_Destory(QueuePtr queue)
{
	free(queue->payload);
	queue->payload = NULL;
	free(queue);
	queue = NULL;
}

/**
 * ��ָ���˺�len���ֽڣ����������ã�����
 * @param queue ���о��
 * @param len   Ҫ�˺���ֽ���
 */
void Queue_ErrBack(const QueuePtr queue, uint32_t len)
{
	int tmp = queue->read - len;
	queue->read = tmp < 0 ? ((uint32_t)(tmp + queue->size)) : ((uint32_t)tmp);
}



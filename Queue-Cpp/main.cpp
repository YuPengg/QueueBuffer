/*
 * main.cpp
 *
 *  Created on: 2020��10��12��
 *      Author: hello
 */


#include <iostream>
#include <stdio.h>
#include <string.h>
#include "cqueue.h"

using namespace std;

int main()
{
	char buf[20];

	// ʵ������
	CQueue queue(20);

	// �������
	memset(buf, 0X00, sizeof(buf));

	// ��
	queue.write("helloworld", 10);

	// д
	queue.read(buf, 10);

	// ��ӡ
	printf("read:%s\n", buf);

	return 0;
}


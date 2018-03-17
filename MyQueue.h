#pragma once
#include "Packet.h"
#include <iostream>
using namespace std;

class MyQueue
{
public:
	MyQueue();
	MyQueue(int capacity);
	virtual ~MyQueue();
	void push(Packet* packet);
	void pop();
	Packet* front();
	bool full();
	bool empty();
	void setHead(Packet* head);//����ͷָ��
	Packet* getHead();//��ȡͷָ��
	void setTail(Packet* tail);//����βָ��
	Packet* getTail();//��ȡβָ��
	int size();//��ȡ���д�С
	void setSize(int capacity);//���ö��д�С
	float getLoad();
	int getNowLoad();
	int getLeave();
private:
	int capacity;//���д�С
	int load;//���е�ǰ����
	Packet* head;//ͷָ��
	Packet* tail;//βָ��
};


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
	void setHead(Packet* head);//设置头指针
	Packet* getHead();//获取头指针
	void setTail(Packet* tail);//设置尾指针
	Packet* getTail();//获取尾指针
	int size();//获取队列大小
	void setSize(int capacity);//设置队列大小
	float getLoad();
	int getNowLoad();
	int getLeave();
private:
	int capacity;//队列大小
	int load;//队列当前负载
	Packet* head;//头指针
	Packet* tail;//尾指针
};


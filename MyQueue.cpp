#include "stdafx.h"
#include "MyQueue.h"


MyQueue::MyQueue()
{
	load = 0;
	head = NULL;
	tail = NULL;
}

MyQueue::MyQueue(int capacity)
{
	this->capacity = capacity;
	load = 0;
	head = NULL;
	tail = NULL;
}

MyQueue::~MyQueue()
{
}

void MyQueue::setHead(Packet* head)
{
	this->head = head;
}

Packet* MyQueue::getHead()
{
	return head;
}

void MyQueue::setTail(Packet* tail)
{
	this->tail;
}

Packet* MyQueue::getTail()
{
	return tail;
}

bool MyQueue::full()
{
	if (load == capacity)
		return true;
	else
		return false;
}

bool MyQueue::empty()
{
	if (load == 0)
		return true;
	else
		return false;
}

void MyQueue::push(Packet* packet)
{
	if (full())
		return;
	else
	{
		if (empty())
		{
			head = packet;
			tail = packet;
		}
		else
		{
			tail->setNext(packet);
			tail = packet;
		}
		load++;
	}
}

void MyQueue::pop()
{
	if (empty())
		return;
	else
	{
		if (load == 1)
		{
			head = NULL;
			tail = NULL;
		}
		else
			head = head->getNext();

		load--;
	}
}

Packet* MyQueue::front()
{
	if (empty())
		return NULL;
	else
		return head;
}

int MyQueue::size()
{
	return capacity;
}

void MyQueue::setSize(int capacity)
{
	this->capacity = capacity;
}

float MyQueue::getLoad()
{
	float nowLoad = (float)load / (float)capacity;
	return nowLoad;
}

int MyQueue::getNowLoad()
{
	return load;
}

int MyQueue::getLeave()
{
	return (capacity - load);
}
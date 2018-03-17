#include "stdafx.h"
#include "Packet.h"


Packet::Packet(int priority)
{
	this->priority = priority;
	nextPacket = NULL;
}


Packet::~Packet()
{
}

void Packet::born()
{
	bornTime = GetTickCount();
}

void Packet::dead()
{
	deadTime = GetTickCount();
}

void Packet::setNext(Packet* nextPacket)
{
	this->nextPacket = nextPacket;
}

Packet* Packet::getNext()
{
	return nextPacket;
}

int Packet::getPriority()
{
	return priority;
}

double Packet::getDelay()
{
	accumulatedDelay = deadTime - bornTime;
	return accumulatedDelay;
}

void Packet::setChannel(int channel)
{
	this->channel = channel;
}

int Packet::getChannel()
{
	return channel;
}

void Packet::setLocation(int location)
{
	this->location = location;
}

int Packet::getLocation()
{
	return location;
}
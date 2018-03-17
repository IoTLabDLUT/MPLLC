#include "stdafx.h"
#include "Global.h"

queue<Packet*> sendPacketList;
queue<Packet*> receivePacketList;
MyQueue* channelReceiveQueue;
mutex rpl_mtx;
mutex spl_mtx;
int packetCounter = 1;
mutex pc_mtx;
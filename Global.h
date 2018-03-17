#pragma once
#include <queue>
#include "MyQueue.h"
#include "Packet.h"
#include <mutex>
#include <iostream>
#include <fstream>
using namespace std;

extern queue<Packet*> sendPacketList;//上层发送队列
extern queue<Packet*> receivePacketList;//上层接收队列
extern MyQueue* channelReceiveQueue;
extern mutex rpl_mtx;
extern mutex spl_mtx;
extern int packetCounter;
extern mutex pc_mtx;
extern queue<Packet*> exPacketList;
extern mutex epl_mtx;
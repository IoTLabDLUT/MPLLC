// MPLLC.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Global.h"
#include "Channel.h"
#include "MyQueue.h"
#include <windows.h>
#include <thread>
#include <mutex>
#include <pthread.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include <fstream>
using namespace std;
#pragma comment(lib, "x86/pthreadVC2.lib")

queue<int> test_queue;
MyQueue myQueue;
mutex mtx;

ofstream cOut("e:/data/channelC.txt");

int counter = 0;
double RTT_LTE = 51000;//LTE最大延迟
double RTT_80211P = 11000;//802.11p最大延迟

Channel* channel1 = new Channel(20, 10, 500, 5000, 50, 1);
Channel* channel2 = new Channel(100, 10, 500, 5000, 50, 2);

void generator(int totalNum, int secNum)
{
	int secCounter = 0;
	int secProp = (totalNum / secNum);//安全类的包占的比重
	for (int i = 0; i < totalNum; i++)
	{
		Packet* packet = NULL;
		secCounter++;
		if (secCounter == secProp)
		{
			secCounter = 0;
			packet = new Packet(0);
		}
		else
		{
			packet = new Packet((rand() % 3) + 1);
		}
		sendPacketList.push(packet);
	}
}

double getChannelC(Channel* channel, Packet* packet)
{
	double beta = (double)(packet->getPriority() + 1) / 5;//计算优先级因子
	double rttEle, loadEle;
	double channelC;
	MyQueue* tempSendQueue = channel->getSendQueue();

	channel->send_mtx.lock();
	channel->rtt_mtx.lock();
	if (channel->getNum() == 1)
	{
		rttEle = channel->getRTT() / RTT_80211P;//计算rtt因子
		loadEle = (double)tempSendQueue->getNowLoad() / (double)(tempSendQueue->size() - 20);//计算负载因子
	}
	else
	{
		rttEle = channel->getRTT() / RTT_LTE;//计算rtt因子
		loadEle = (double)tempSendQueue->getNowLoad() / (double)tempSendQueue->size();//计算负载因子
	}
	channelC = beta * rttEle + (1 - beta) * loadEle;
	channel->rtt_mtx.unlock();
	channel->send_mtx.unlock();

	return channelC;
}

void subSelect(Channel* channel, Packet* packet)
{
	channel->send_mtx.lock();
	if (!(channel->getSendQueue())->full())
	{
		spl_mtx.lock();
		sendPacketList.pop();
		spl_mtx.unlock();

		packet->setChannel(channel->getNum());
		(channel->getSendQueue())->push(packet);
		packet->born();
		packet->setLocation((channel->getSendQueue())->getNowLoad());
	}
	channel->send_mtx.unlock();
}

void channelSelect_1()//轮询算法
{
	Channel* channel = NULL;

	while (true)
	{
		spl_mtx.lock();
		if (!sendPacketList.empty())
		{
			Packet* packet = sendPacketList.front();
			spl_mtx.unlock();

			if (packet->getPriority() == 0)
			{
				channel = channel1;
			}
			else
			{
				if (counter % 2 == 0)
					channel = channel1;
				else
					channel = channel2;
				counter++;
			}
			subSelect(channel, packet);
		}
		else
		{
			spl_mtx.unlock();

			pc_mtx.lock();
			if (packetCounter > 5000)
			{
				pc_mtx.unlock();
				return;
			}
			else
				pc_mtx.unlock();
		}
		//Sleep(5);
	}
}

void channelSelect_2()//目前车联网使用的算法
{
	Channel* channel = NULL;

	while (true)
	{
		spl_mtx.lock();
		if (!sendPacketList.empty())
		{
			Packet* packet = sendPacketList.front();
			spl_mtx.unlock();

			if (packet->getPriority() == 0)
				channel = channel1;
			else
				channel = channel2;

			subSelect(channel, packet);
		}
		else
		{
			spl_mtx.unlock();

			pc_mtx.lock();
			if (packetCounter > 5000)
			{
				pc_mtx.unlock();
				return;
			}
			else
				pc_mtx.unlock();
		}

		//Sleep(5);
	}
}

void channelSelect_3()
{
	Channel* channel = NULL;
	
	while (true)
	{
		spl_mtx.lock();
		if (!sendPacketList.empty())
		{
			Packet* packet = sendPacketList.front();
			spl_mtx.unlock();

			if (packet->getPriority() == 0)
				channel = channel1;
			else
			{
				cOut << getChannelC(channel1, packet) << "  " << getChannelC(channel2, packet) << endl;
				if (getChannelC(channel1, packet) < getChannelC(channel2, packet))
					channel = channel1;
				else
					channel = channel2;
			}

			subSelect(channel, packet);
		}
		else
		{
			spl_mtx.unlock();

			pc_mtx.lock();
			if (packetCounter > 5000)
			{
				pc_mtx.unlock();
				return;
			}
			else
				pc_mtx.unlock();
		}

		//Sleep(5);
	}
}

void testTime()
{
	while (true)
	{
		epl_mtx.lock();
		if (!exPacketList.empty())
		{
			Packet* packet = exPacketList.front();
			epl_mtx.unlock();
			subSelect(channel1, packet);
		}
		else
		{
			epl_mtx.unlock();

			pc_mtx.lock();
			if (packetCounter > 5000)
			{
				pc_mtx.unlock();
				return;
			}
			else
				pc_mtx.unlock();
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	long lastTime;//程序持续时间
	generator(5000, 200);

	queue<Packet*> priority_0;
	queue<Packet*> priority_1;
	queue<Packet*> priority_2;
	queue<Packet*> priority_3;

	ofstream delayOut("e:/testD.txt");
	ofstream locOut("e:/testL.txt");

	DWORD startTime = GetTickCount();//程序开始时间

	thread thread_select(channelSelect_3);
	channel1->start();
	channel2->start();

	thread_select.join();

	while (channel1->getState() || channel2->getState())
	{}

	DWORD overTime = GetTickCount();//程序结束时间
	lastTime = overTime - startTime;


	//数据输出和统计
	while (!receivePacketList.empty())
	{
		Packet* pack = receivePacketList.front();
		receivePacketList.pop();

		if (pack->getChannel() == 1)
			locOut << pack->getLocation() << endl;//802.11p信道利用率

		switch (pack->getPriority())
		{
		    case 0:
		    {
				  priority_0.push(pack);
				  break;
		    }
		    case 1:
		    {
				  priority_1.push(pack);
				  break;
		    }
		    case 2:
		    {
				  priority_2.push(pack);
				  break;
		    }
		    case 3:
		    {
				  priority_3.push(pack);
				  break;
		    }
		}
	}

	for (int i = 0; i < 4; i++)
	{
		Packet* tempPacket = NULL;
		if (i == 0)
		{
			while (!priority_0.empty())
			{
				tempPacket = priority_0.front();
				delayOut << tempPacket->getDelay() << endl;
				priority_0.pop();
			}
		}
		else if (i == 1)
		{
			while (!priority_1.empty())
			{
				tempPacket = priority_1.front();
				delayOut << tempPacket->getDelay() << endl;
				priority_1.pop();
			}
		}
		else if (i == 2)
		{
			while (!priority_2.empty())
			{
				tempPacket = priority_2.front();
				delayOut << tempPacket->getDelay() << endl;
				priority_2.pop();
			}
		}
		else
		{
			while (!priority_3.empty())
			{
				tempPacket = priority_3.front();
				delayOut << tempPacket->getDelay() << endl;
				priority_3.pop();
			}
		}
		delayOut << endl;
	}
	delayOut << "Channel1:" << channel1->getSendCounter() << "  Channel2:" << channel2->getSendCounter() << endl;
	delayOut << "LastTime:" << lastTime << endl;
	

	delayOut.close();
	locOut.close();
	cOut.close();

	system("pause");
	return 0;
}


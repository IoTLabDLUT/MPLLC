#include "stdafx.h"
#include "Channel.h"


Channel::Channel()
{
}

Channel::Channel(int sendRate, int processRate, int sendSize, int receiveSize, int transmitDelay, int channelNum)
{
	this->sendRate = sendRate;
	this->transmitDelay = transmitDelay;
	this->processRate = processRate;
	sendQueue = new MyQueue(sendSize);
	receiveQueue = new MyQueue(receiveSize);
	this->isOn = false;
	this->channelNum = channelNum;
	sendCounter = 0;
	lossCounter = 0;
	expectedRTT = 0;
	largeRTT = 0;
}


Channel::~Channel()
{
}

void Channel::setRTT(double expectedRTT)
{
	this->expectedRTT = expectedRTT;
}

double Channel::getRTT()
{
	return expectedRTT;
}

int Channel::getNum()
{
	return channelNum;
}

MyQueue* Channel::getSendQueue()
{
	return sendQueue;
}

void Channel::transmit()
{
	while (true)
	{
		Packet* packet = NULL;
		send_mtx.lock();
		if (!sendQueue->empty())
		{
			sendCounter++;
			packet = sendQueue->front();
			sendQueue->pop();
			send_mtx.unlock();

			Sleep(sendRate);

			receive_mtx.lock();
			if (!receiveQueue->full())
			{
				receiveQueue->push(packet);
				packet->dead();

				rtt_mtx.lock();
				setRTT(packet->getDelay());
				rtt_mtx.unlock();

				if (largeRTT < packet->getDelay())
					largeRTT = packet->getDelay();

				cout << "Push into queue" << channelNum << endl;
			}
			receive_mtx.unlock();
		}
		else
		{
			send_mtx.unlock();

			pc_mtx.lock();
			if (packetCounter > 5000)
			{
				pc_mtx.unlock();
				return;
			}
			else
				pc_mtx.unlock();
		}

		//Sleep(sendRate);
	}
}

void Channel::postProcess()
{
	while (true)
	{
		Packet* packet = NULL;
		receive_mtx.lock();
		if (!receiveQueue->empty())
		{
			packet = receiveQueue->front();
			receiveQueue->pop();

			if (packet != NULL)
			{
				rpl_mtx.lock();
				receivePacketList.push(packet);
				cout << "Pop out of queue" << channelNum << endl;
				rpl_mtx.unlock();

				Sleep(processRate);
			}
			receive_mtx.unlock();

			pc_mtx.lock();
			packetCounter++;
			pc_mtx.unlock();
		}
		else
		{
			receive_mtx.unlock();

			pc_mtx.lock();
			if (packetCounter > 5000)
			{
				pc_mtx.unlock();
				over();
				return;
			}
			else
				pc_mtx.unlock();
		}
	}
}

void Channel::start()
{
	isOn = true;

	thread thread1(&Channel::transmit, this);
	thread thread2(&Channel::postProcess, this);

	thread1.detach();
	thread2.detach();
}

void Channel::over()
{
	isOn = false;
}

MyQueue* Channel::getReceiveQueue()
{
	return receiveQueue;
}

bool Channel::getState()
{
	return isOn;
}

int Channel::getSendCounter()
{
	return sendCounter;
}

double Channel::getLargeRTT()
{
	return largeRTT;
}
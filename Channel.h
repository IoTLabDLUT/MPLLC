#pragma once
#include "MyQueue.h"
#include "Global.h"
#include <thread>
#include <mutex>
#include <iostream>
using namespace std;

class Channel
{
public:
	Channel();
	Channel(int sendRate, int processRate, int sendSize, int receiveSize, int transmitDelay, int channelNum);
	virtual ~Channel();
	void setRTT(double expectedRTT);//����RTT
	double getRTT();//��ȡRTT
	int getNum();//��ȡ�ŵ�����
	MyQueue* getSendQueue();//��ȡ���Ͷ���
	MyQueue* getReceiveQueue();
	void transmit();//������Ϣ���䴦��
	void postProcess();//�������ݺ���д���
	void start();//�ŵ���ʼ
	void over();//�ŵ�����
	bool getState();
	int getSendCounter();
	double getLargeRTT();
	mutex send_mtx;//���Ͷ�����
	mutex receive_mtx;//���ն�����
	mutex rtt_mtx;
private:
	int sendRate;//����Ƶ��
	int transmitDelay;//�����ӳ�
	MyQueue* sendQueue;//���Ͷ���
	MyQueue* receiveQueue;//���ն���
	double expectedRTT;//��ʷRTTֵ
	int processRate;//���ն������ϲ��ύ��Ƶ��
	bool isOn;//�ŵ��Ƿ����
	int channelNum;//�ŵ�����
	int sendCounter;//������
	int lossCounter;//������
	double largeRTT;//���RTT
};


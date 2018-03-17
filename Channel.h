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
	void setRTT(double expectedRTT);//设置RTT
	double getRTT();//获取RTT
	int getNum();//获取信道号码
	MyQueue* getSendQueue();//获取发送队列
	MyQueue* getReceiveQueue();
	void transmit();//进行信息传输处理
	void postProcess();//接收数据后进行处理
	void start();//信道开始
	void over();//信道结束
	bool getState();
	int getSendCounter();
	double getLargeRTT();
	mutex send_mtx;//发送队列锁
	mutex receive_mtx;//接收队列锁
	mutex rtt_mtx;
private:
	int sendRate;//发送频率
	int transmitDelay;//传输延迟
	MyQueue* sendQueue;//发送队列
	MyQueue* receiveQueue;//接收队列
	double expectedRTT;//历史RTT值
	int processRate;//接收队列向上层提交的频率
	bool isOn;//信道是否可用
	int channelNum;//信道号码
	int sendCounter;//发包数
	int lossCounter;//丢包数
	double largeRTT;//最大RTT
};


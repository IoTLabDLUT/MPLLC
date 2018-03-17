#pragma once
#include <windows.h>
#include <iostream>
using namespace std;

class Packet
{
public:
	Packet(int priority);
	virtual ~Packet();
	void born();//送入队列时调用，记录包生成时间
	void dead();//弹出队列时调用，记录包死亡时间
	void setNext(Packet* nextPacket);//设置nextPacket
	Packet* getNext();//获取nextPacket
	int getPriority();//获取包优先级
	double getDelay();//获取包的延迟
	void setChannel(int channel);//设置选择的信道
	int getChannel();//获取选择的信道
	void setLocation(int location);
	int getLocation();
private:
	DWORD bornTime;//送入信道时间
	DWORD deadTime;//离开信道时间
	int priority;//优先级
	int channel;//选择的信道
	double accumulatedDelay;//传输延迟
	Packet* nextPacket;//下一个包
	int location;
};


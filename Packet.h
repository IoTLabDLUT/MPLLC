#pragma once
#include <windows.h>
#include <iostream>
using namespace std;

class Packet
{
public:
	Packet(int priority);
	virtual ~Packet();
	void born();//�������ʱ���ã���¼������ʱ��
	void dead();//��������ʱ���ã���¼������ʱ��
	void setNext(Packet* nextPacket);//����nextPacket
	Packet* getNext();//��ȡnextPacket
	int getPriority();//��ȡ�����ȼ�
	double getDelay();//��ȡ�����ӳ�
	void setChannel(int channel);//����ѡ����ŵ�
	int getChannel();//��ȡѡ����ŵ�
	void setLocation(int location);
	int getLocation();
private:
	DWORD bornTime;//�����ŵ�ʱ��
	DWORD deadTime;//�뿪�ŵ�ʱ��
	int priority;//���ȼ�
	int channel;//ѡ����ŵ�
	double accumulatedDelay;//�����ӳ�
	Packet* nextPacket;//��һ����
	int location;
};


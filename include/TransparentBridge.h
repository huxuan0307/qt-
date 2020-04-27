#ifndef _TRANSPARENT_BRIDGE_
#define _TRANSPARENT_BRIDGE_

#include <unordered_map>
#include <map>
#include <iostream>
#include <vector>
#include <set>
#include <Windows.h>
using std::string;
using std::vector;
using std::set;
using std::unordered_map;


namespace LinkProtocal
{
	struct MessageFrame
	{
		string sourceName;
		string sourceMAC;
		string targetMAC;
		string message;
	};

	class TransparentBridge
	{
	public:
		struct Data//������ṹ
		{
			int   port;		//�˿�
			int   sec;		//�ϴ���Ϣʱ��
		};
	private:
		int timeout = 120;	//Ĭ�ϳ�ʱʱ�䣨�룩
		vector<int> ports;	//�˿ں�����
		unordered_map<string, Data> SendMap;	//ת����<MAC, <port, time>>
	public:
		TransparentBridge () { }
		~TransparentBridge () { }
		// ���ö˿ں�
		void setPort (vector<int> ports);
		// ��ȡ�˿ں�
		int getPort (int index);
		// ��ȡת����
		unordered_map<string, Data>const& getSendMap ()const;
		// ���Ҷ˿ں��Ƿ������������
		bool findPort (int targetPort);
		// ����ʱ�䣬����ת����
		void TimeTick ();
		// ��ת�����в��Ҷ�Ӧ�Ķ˿ں�
		int findPortInSendMap (string MAC);
		// ������Ϣ
		void recvMessage (MessageFrame msgf, int recvPort);
		// ת����Ϣ
		void sendMessage (MessageFrame msgf, int recvPort);
	};

	extern int maxport;
	// portΪ����source MACΪֵ

	extern unordered_map<int, set<int>>		wires;	// һ�������
	extern unordered_map<int, vector<MessageFrame>>  messagePoll;



	class PC
	{
	public:
		vector<string> recvedMessage;						//ĳʱ�̽��յ�����Ϣ
		vector<string> senderMAC;							//����Ϣ��Ӧ�ķ��ͷ�MAC
		vector<string> senderName;							//���ͷ����ƣ�Ϊ���ӻ�׼��
		string name;										//��PC���ƣ�Ϊ���ӻ�׼��
		string MAC;
		int port;
		PC (string _MAC, int port);
		bool findPort (int targetPort);						//���Ҹö˿��Ƿ����ڴ�PC
		void recvMessage (MessageFrame msgf, int recvPort);	//������Ϣ
		void reply (string targetMAC);						//��Ӧ��Ϣ
		void sendMessage (string targetMAC, string msg);	//������Ϣ
		void clearMessage ();								//�����Ϣ����
	private:
		void printMessage (MessageFrame msgf);
	};

	extern vector<PC>					PCv;
	extern vector<TransparentBridge>	TBv;

	void recvMessage ();

	TransparentBridge addTB ();

	PC addPC (string MAC, int port, string name="");

	void addwire (int port1, int port2);

	void timetick ();
	void setMessage (int sourcePort, MessageFrame msgf);
	bool hasMessageBetweenPorts (int port1, int port2);

};


#endif // !_TRANSPARENT_BRIDGE_




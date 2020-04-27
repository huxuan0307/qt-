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
		struct Data//数据项结构
		{
			int   port;		//端口
			int   sec;		//上次消息时间
		};
	private:
		int timeout = 120;	//默认超时时间（秒）
		vector<int> ports;	//端口号数组
		unordered_map<string, Data> SendMap;	//转发表<MAC, <port, time>>
	public:
		TransparentBridge () { }
		~TransparentBridge () { }
		// 设置端口号
		void setPort (vector<int> ports);
		// 获取端口号
		int getPort (int index);
		// 获取转发表
		unordered_map<string, Data>const& getSendMap ()const;
		// 查找端口号是否属于这个网桥
		bool findPort (int targetPort);
		// 更新时间，更新转发表
		void TimeTick ();
		// 在转发表中查找对应的端口号
		int findPortInSendMap (string MAC);
		// 接收消息
		void recvMessage (MessageFrame msgf, int recvPort);
		// 转发消息
		void sendMessage (MessageFrame msgf, int recvPort);
	};

	extern int maxport;
	// port为键，source MAC为值

	extern unordered_map<int, set<int>>		wires;	// 一口连多口
	extern unordered_map<int, vector<MessageFrame>>  messagePoll;



	class PC
	{
	public:
		vector<string> recvedMessage;						//某时刻接收到的消息
		vector<string> senderMAC;							//与消息对应的发送方MAC
		vector<string> senderName;							//发送方名称，为可视化准备
		string name;										//此PC名称，为可视化准备
		string MAC;
		int port;
		PC (string _MAC, int port);
		bool findPort (int targetPort);						//查找该端口是否属于此PC
		void recvMessage (MessageFrame msgf, int recvPort);	//接收消息
		void reply (string targetMAC);						//回应消息
		void sendMessage (string targetMAC, string msg);	//发送消息
		void clearMessage ();								//清空消息队列
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




#include "TransparentBridge.h"
using namespace std;
#define DEBUG

namespace LinkProtocal
{
	// ���ö˿�

	inline void TransparentBridge::setPort (vector<int> ports)
	{
		this->ports = ports;
	}

	// ��ȡ�˿�

	int TransparentBridge::getPort (int index)
	{
		return ports[index];
	}
	using Data = TransparentBridge::Data;

	unordered_map<string, Data>const& TransparentBridge::getSendMap () const
	{
		return SendMap;
	}

	// ���Ҷ˿ں��Ƿ������������

	inline bool TransparentBridge::findPort (int targetPort)
	{
		for (auto& port : ports) {
			if (targetPort == port)
				return true;
		}
		return false;
	}

	// ÿ��һ�����ʱ��

	inline void TransparentBridge::TimeTick ()
	{
		vector<string> tmp;
		for (auto& itm : SendMap) {
			if (--itm.second.sec < 0) {
				// ��¼��ʱ��Ҫ��ת�����Ƴ�����Ϣ
				tmp.push_back (itm.first);
			}
		}
		// �Ƴ�
		for (auto& itm : tmp) {
			SendMap.erase (itm);
		}
	}

	inline int TransparentBridge::findPortInSendMap (string MAC)
	{
		if (SendMap.count (MAC)) {
			return SendMap[MAC].port;
		}
		return -1;
	}

	inline void LinkProtocal::TransparentBridge::recvMessage (MessageFrame msgf, int recvPort)
	{
		if (findPortInSendMap (msgf.sourceMAC) == -1) {
			// ·�ɱ���û�д�ԴMAC�����
			SendMap[msgf.sourceMAC].port = recvPort;
			// ���ó�ʱʱ��
			SendMap[msgf.sourceMAC].sec = timeout;
		}
		else {
			// ·�ɱ����д�ԴMAC
			if (recvPort == SendMap[msgf.sourceMAC].port) {
				// port��ͬ���޸�
				SendMap[msgf.sourceMAC].port = recvPort;
			}
			// ���ó�ʱʱ��
			SendMap[msgf.sourceMAC].sec = timeout;
		}
		// ת����Ϣ
		sendMessage (msgf, recvPort);

	}



	inline void LinkProtocal::TransparentBridge::sendMessage (MessageFrame msgf, int recvPort)
	{
		int targetPort = findPortInSendMap (msgf.targetMAC);
		if (targetPort == recvPort)
			return;
		if (targetPort == -1) {
			// ת������û��-->����
			for (auto& port : ports) {
				// �ų���Ϣ��Դ�Ķ˿�
				if (port != recvPort) {
					// ����Ϣ��������Ϣ
					// �����˿ں�&ԭʼ��Ϣ
					setMessage (port, msgf);
				}
			}
		}
		else {
			// ת��������-->��ָ���˿�Ͷ��
			setMessage (targetPort, msgf);
		}
	}

	inline LinkProtocal::PC::PC (string _MAC, int port) :MAC (_MAC), port (port) { }

	inline bool LinkProtocal::PC::findPort (int targetPort)
	{
		if (port == targetPort)
			return true;
		return false;
	}

	inline void LinkProtocal::PC::recvMessage (MessageFrame msgf, int recvPort)
	{
		if (MAC != msgf.targetMAC)
			return;
		this->recvedMessage.push_back (msgf.message);
	}

	inline void LinkProtocal::PC::reply (string targetMAC)
	{
		MessageFrame msgf;
		msgf.targetMAC = targetMAC;
		msgf.sourceMAC = this->MAC;
		setMessage (port, msgf);
	}

	void LinkProtocal::PC::sendMessage (string targetMAC, string msg)
	{
		MessageFrame msgf;
		msgf.message = msg;
		msgf.sourceMAC = this->MAC;
		msgf.targetMAC = targetMAC;
		setMessage (port, msgf);
	}

	void PC::clearMessage ()
	{ 
		recvedMessage.clear ();
		senderMAC.clear ();
	}

	inline void LinkProtocal::PC::printMessage (MessageFrame msgf)
	{
		cout << msgf.message << endl;
	}

	void LinkProtocal::recvMessage ()
	{
		vector<int> tmp_msgfv;
		auto messagePollClone = messagePoll;
		messagePoll.clear ();
		for (auto& port_messages : messagePollClone) {
			auto& targetPort = port_messages.first;
			auto& msgfs = port_messages.second;

			for (auto& msgf : msgfs) {
				for (auto& TB : TBv) {
					// ���˿��Ƿ����ڴ�����
					if (TB.findPort (targetPort)) {
						TB.recvMessage (msgf, targetPort);
					}
				}
				for (auto& pc : PCv) {
					if (pc.MAC == msgf.sourceMAC)
						continue;
					if (pc.port == targetPort) {
						pc.recvMessage (msgf, targetPort);
						pc.senderMAC.push_back (msgf.sourceMAC);
						pc.senderName.push_back (msgf.sourceName);
					}
				}

			}

		}
	}

	int maxport;
	// portΪ����source MACΪֵ

	unordered_map<int, set<int>>		wires;	// һ�������
	unordered_map<int, vector<MessageFrame>>  messagePoll;
	vector<PC>					PCv;
	vector<TransparentBridge>	TBv;


	TransparentBridge LinkProtocal::addTB ()
	{
		TransparentBridge TBtmp;
		vector<int> porttmp;
		// ���ɶ˿ںţ�������Ϣʱ��ѯ
		for (size_t i = 0; i < 4; i++) {
			porttmp.push_back (++maxport);
		}
		TBtmp.setPort (porttmp);
		TBv.push_back (TBtmp);
		return TBtmp;
	}

	PC LinkProtocal::addPC (string MAC, int port, string name)
	{
		PC PCtmp (MAC, port);
		PCtmp.name = name;
		PCv.push_back (PCtmp);
		return PCtmp;
	}

	void LinkProtocal::addwire (int port1, int port2)
	{
		for (auto& port : wires[port2]) {
			// �Ѿ���port2�����Ķ˿�
			if (port == port1)
				continue;
			wires[port1].insert (port);
		}
		wires[port1].insert (port2);
		for (auto& port : wires[port1]) {
			// �Ѿ���port1�����Ķ˿�
			if (port == port2)
				continue;
			wires[port2].insert (port);
			wires[port].insert (port2);
		}
		wires[port2].insert (port1);
	}

	void LinkProtocal::timetick ()
	{
		for (auto& tb : TBv) {
			tb.TimeTick ();
		}
	}

	void setMessage (int sourcePort, MessageFrame msgf)
	{
		for (auto& port : wires[sourcePort]) {
			messagePoll[port].push_back (msgf);
		}
	}

	bool hasMessageBetweenPorts (int port1, int port2)
	{
		return messagePoll.count (port1) || messagePoll.count (port2);
	}

}



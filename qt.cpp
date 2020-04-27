#include "qt.h"
#include <functional>
#include <Windows.h>
#include <qdebug>
#include <fstream>
using LinkProtocal::addPC;
using LinkProtocal::addTB;
using LinkProtocal::addwire;
using LinkProtocal::TBv;
using LinkProtocal::PCv;
using LinkProtocal::recvMessage;
using std::bind;
using LinkProtocal::timetick;
using LinkProtocal::setMessage;
using LinkProtocal::MessageFrame;
using std::fstream;
const int NEXTSTEPTIME = 80;


void qt::readMACfromfile ()
{
	fstream fin ("./MAC.config", std::ios::in);
	fin >> PCMAC0;
	fin >> PCMAC1;
	fin >> PCMAC2;
	fin >> PCMAC3;
	fin >> PCMAC4;
	fin >> PCMAC5;
}

qt::qt (QWidget* parent)
	: QMainWindow (parent)
{
	ui.setupUi (this);
	paintpointer = new Painter (ui.graphicsView);
	readMACfromfile ();
	cnt = 0;
	// ���2̨����
	addTB ();
	addTB ();

	// ���6̨PC
	addPC (PCMAC0, 1001, "PC1");
	addPC (PCMAC1, 1002, "PC2");
	addPC (PCMAC2, 1003, "PC3");
	addPC (PCMAC3, 1004, "PC4");
	addPC (PCMAC4, 1005, "PC5");
	addPC (PCMAC5, 1006, "PC6");

	addwire (TBv[0].getPort (0), PCv[0].port);
	addwire (TBv[0].getPort (1), PCv[1].port);
	addwire (TBv[0].getPort (2), PCv[2].port);
	addwire (TBv[0].getPort (3), TBv[1].getPort (0));
	addwire (TBv[1].getPort (1), PCv[3].port);
	addwire (TBv[1].getPort (2), PCv[4].port);
	addwire (TBv[1].getPort (3), PCv[5].port);
	init ();
	main_timer = new QTimer (this);
	step_timer = new QTimer (this);
	connect (main_timer, &QTimer::timeout, this, &qt::every50msdo);

	main_timer->start (50);
}

void qt::init ()
{
	connect (ui.PC1_toolButton, &QToolButton::clicked, this, bind (&qt::showDetailInfoPC, this, ui.PC1_toolButton, PCv[0]));
	connect (ui.PC2_toolButton, &QToolButton::clicked, this, bind (&qt::showDetailInfoPC, this, ui.PC2_toolButton, PCv[1]));
	connect (ui.PC3_toolButton, &QToolButton::clicked, this, bind (&qt::showDetailInfoPC, this, ui.PC3_toolButton, PCv[2]));
	connect (ui.PC4_toolButton, &QToolButton::clicked, this, bind (&qt::showDetailInfoPC, this, ui.PC4_toolButton, PCv[3]));
	connect (ui.PC5_toolButton, &QToolButton::clicked, this, bind (&qt::showDetailInfoPC, this, ui.PC5_toolButton, PCv[4]));
	connect (ui.PC6_toolButton, &QToolButton::clicked, this, bind (&qt::showDetailInfoPC, this, ui.PC6_toolButton, PCv[5]));

	//connect (ui.nextStep_toolButton, &QToolButton::clicked, this, &qt::nextStep);
	connect (ui.goAhead_toolButton, &QToolButton::clicked, this, &qt::goAhead);

	connect (ui.sendMessage_toolButton, &QToolButton::clicked, this, &qt::sendMessage_toolButtonclicked);
	connect (ui.send_comboBox, QOverload<int>::of (&QComboBox::currentIndexChanged), this, bind (&qt::syncPC_MAC, this, ui.send_comboBox, ui.senderMAC_lineEdit));
	connect (ui.recv_comboBox, QOverload<int>::of (&QComboBox::currentIndexChanged), this, bind (&qt::syncPC_MAC, this, ui.recv_comboBox, ui.recverMAC_lineEdit));
	syncPC_MAC (ui.send_comboBox, ui.senderMAC_lineEdit);
	syncPC_MAC (ui.recv_comboBox, ui.recverMAC_lineEdit);
	showDetailInfoTB (ui.TB1_toolButton, TBv[0], ui.TB1_detailTable);
	showDetailInfoTB (ui.TB2_toolButton, TBv[1], ui.TB2_detailTable);
	//while (!paintpointer->ready)
	//	;
	qtoolbuttonv.push_back (ui.PC1_toolButton);
	qtoolbuttonv.push_back (ui.PC2_toolButton);
	qtoolbuttonv.push_back (ui.PC3_toolButton);
	qtoolbuttonv.push_back (ui.PC4_toolButton);
	qtoolbuttonv.push_back (ui.PC5_toolButton);
	qtoolbuttonv.push_back (ui.PC6_toolButton);

	paintpointer->lastTime = QTime::currentTime ();
}

void qt::every50msdo ()
{
	++cnt;
	QTime time = QTime::currentTime ();
	ui.timeEdit->setTime (time);
	if (cnt % NEXTSTEPTIME == 0) {
		recvMessage ();
		this->paintpointer->stateCnt++;
		this->paintpointer->updateLineStateMap ();
		qDebug () << "state=" << this->paintpointer->stateCnt;

		//ui.messageBox->textCursor ().insertText (QString ("state:") + QString::number (this->paintpointer->stateCnt));
		for (auto iter = this->paintpointer->lineStateMap.keyBegin (); iter != this->paintpointer->lineStateMap.keyEnd (); ++iter) {
			qDebug () << '[' << iter->first << ',' << iter->second << ']' << this->paintpointer->lineStateMap[*iter];
		}

	}
	if (cnt % 20 == 0) {
		timetick ();
		showDetailInfoTB (ui.TB1_toolButton, TBv[0], ui.TB1_detailTable);
		showDetailInfoTB (ui.TB2_toolButton, TBv[1], ui.TB2_detailTable);

	}
	if (this->paintpointer->stateCnt < 4) {
		qtoolbuttonv.at (ui.send_comboBox->currentIndex ())->setAutoRaise (false);
		qtoolbuttonv.at (ui.recv_comboBox->currentIndex ())->setAutoRaise (false);
	}
	else {
		for (size_t i = 0; i < qtoolbuttonv.size (); i++) {
			qtoolbuttonv[i]->setAutoRaise (true);
		}
	}

	paintpointer->show ();
	paintpointer->repaint ();
	updateMessageInMessageBox ();

}

void qt::sendMessage_toolButtonclicked ()
{
	MessageFrame msgf;
	msgf.sourceMAC = ui.senderMAC_lineEdit->text ().toStdString ();
	msgf.targetMAC = ui.recverMAC_lineEdit->text ().toStdString ();
	if (msgf.sourceMAC == msgf.targetMAC) {
		/*Beep (880, 800);*/
		MessageBeep (MB_ICONWARNING);
		showMessageOnStatusBar ("�շ���MAC��ͬ�����������룡��", 5000);
		return;
	}
	QString msg = ui.message_lineEdit->text ();
	msgf.message = msg.toStdString ();
	if (msgf.message.empty ()) {
		MessageBeep (MB_ICONWARNING);
		showMessageOnStatusBar ("������ϢΪ�գ���������Ϣ�ٷ��ͣ���", 5000);
		return;
	}
	int sendPort = -1;
	for (auto& pc : PCv) {
		if (pc.MAC == msgf.sourceMAC) {
			sendPort = pc.port;
			break;
		}
	}

	if (sendPort == -1) {
		MessageBeep (MB_ICONWARNING);
		showMessageOnStatusBar ("û�кͷ��ͷ�MAC��ͬ��PC�����������룡��", 5000);
		return;
	}
	this->cnt = 0;
	msgf.sourceName = ui.send_comboBox->currentText ().toStdString ();
	setMessage (sendPort, msgf);
	this->paintpointer->stateCnt = 1;
	this->paintpointer->updateLineStateMap ();

	updateMessageInMessageBox ();
	//paintpointer->lastTime = QTime::currentTime ();
	paintpointer->show ();
	paintpointer->repaint ();
}

void qt::showDetailInfoPC (QToolButton* qtb, PC& pc)
{
	QStandardItem* standItem1;
	QStandardItem* standItem2;
	QTableView* tableView = ui.PC_detailTable;
	QStandardItemModel* standItemModel = new QStandardItemModel ();
	standItemModel->setColumnCount (2);
	standItem1 = new QStandardItem (QStringLiteral ("�豸����"));
	standItem2 = new QStandardItem (qtb->text ());
	standItemModel->setItem (0, 0, standItem1);
	standItemModel->setItem (0, 1, standItem2);
	standItem1 = new QStandardItem (QStringLiteral ("MAC"));
	standItem2 = new QStandardItem (QString::fromLocal8Bit (pc.MAC.c_str ()));
	standItemModel->setItem (1, 0, standItem1);
	standItemModel->setItem (1, 1, standItem2);
	tableView->setModel (standItemModel);
	tableView->verticalHeader ()->hide ();							//����Ĭ����ʾ����ͷ
	tableView->horizontalHeader ()->hide ();						//����Ĭ����ʾ����ͷ
	tableView->setSelectionBehavior (QAbstractItemView::SelectRows); //����ѡ��ʱ����ѡ��
	tableView->setEditTriggers (QAbstractItemView::NoEditTriggers);  //���ñ������ֻ�������ܱ༭
	tableView->horizontalHeader ()->setSectionResizeMode (QHeaderView::Stretch);
}

void qt::showDetailInfoTB (QToolButton* qtb, TB& tb, QTableView* tableView)
{
	QStandardItem* standItem1;
	QStandardItem* standItem2;
	QStandardItem* standItem3;

	tableView->destroyed ();
	QStandardItemModel* standItemModel = new QStandardItemModel ();
	standItemModel->setColumnCount (3);
	standItemModel->setHeaderData (0, Qt::Horizontal, QStringLiteral ("MAC"));   //���ñ�ͷ����
	standItemModel->setHeaderData (1, Qt::Horizontal, QStringLiteral ("�˿�"));
	standItemModel->setHeaderData (2, Qt::Horizontal, QStringLiteral ("ʣ��ʱ��"));

	auto SendMap = tb.getSendMap ();
	size_t i = 0;
	QFont font;
	////������������
	//font.setFamily ("����");
	////�������ִ�СΪ50����
	//font.setPixelSize (50);
	//��������Ϊ����
	font.setBold (true);             //��װ��setWeight����

	for (auto& itm : SendMap) {
		// MAC
		auto& MAC = itm.first;
		auto& port = itm.second.port;
		auto& leftTime = itm.second.sec;
		standItem1 = new QStandardItem (MAC.c_str ());
		standItem2 = new QStandardItem (tr ("%1").arg ((port - 1) % 4 + 1));
		standItem3 = new QStandardItem (tr ("%1").arg (leftTime));
		if (this->paintpointer->stateCnt < 4) {
			for (auto& portmsgf : LinkProtocal::messagePoll) {
				if (tb.findPort (portmsgf.first)) {
					for (auto& msg : portmsgf.second) {
						if (msg.targetMAC == MAC) {
							standItem1->setFont (font);
							standItem2->setFont (font);
							standItem3->setFont (font);
						}
					}
				}
			}
		}
		standItemModel->setItem (i, 0, standItem1);
		standItemModel->setItem (i, 1, standItem2);
		standItemModel->setItem (i, 2, standItem3);

		++i;
	}
	tableView->setModel (standItemModel);
	tableView->verticalHeader ()->hide ();							//����Ĭ����ʾ����ͷ
	tableView->setSelectionBehavior (QAbstractItemView::SelectRows); //����ѡ��ʱ����ѡ��
	tableView->setEditTriggers (QAbstractItemView::NoEditTriggers);  //���ñ������ֻ�������ܱ༭
	tableView->horizontalHeader ()->setSectionResizeMode (QHeaderView::Stretch);
}

void qt::goAhead ()
{
	using LinkProtocal::messagePoll;
	while (!messagePoll.empty ())
		recvMessage ();
	size_t sec = ui.spinBox->value ();
	while (sec-- > 0) {
		timetick ();
		ui.timeEdit->setTime (ui.timeEdit->time ().addSecs (1));

	}
	checkPCRecvedMessage ();

	showDetailInfoTB (ui.TB1_toolButton, TBv[0], ui.TB1_detailTable);
	showDetailInfoTB (ui.TB2_toolButton, TBv[1], ui.TB2_detailTable);
	//paintpointer->lastTime = QTime::currentTime ();
	paintpointer->show ();
	paintpointer->repaint ();

}

void qt::nextStep ()
{
	recvMessage ();
	showDetailInfoTB (ui.TB1_toolButton, TBv[0], ui.TB1_detailTable);
	showDetailInfoTB (ui.TB2_toolButton, TBv[1], ui.TB2_detailTable);
	checkPCRecvedMessage ();

	//paintpointer->lastTime = QTime::currentTime ();
	paintpointer->show ();
	paintpointer->repaint ();
}

void qt::syncPC_MAC (QComboBox* qcb, QLineEdit* qle)
{
	qle->setText (QString::fromStdString (PCv[qcb->currentIndex ()].MAC));
}

// ��״̬����ʾ��Ϣ
inline void qt::showMessageOnStatusBar (const string& _Str, const size_t& _RemainTime)const
{
	ui.statusBar->showMessage (QString::fromLocal8Bit (_Str.data ()), _RemainTime);
}


void qt::updateMessageInMessageBox ()
{
	checkPCRecvedMessage ();
}

// 
void qt::checkPCRecvedMessage ()
{
	for (auto& pc : PCv) {
		if (!pc.recvedMessage.empty ()) {
			for (size_t i = 0; i < pc.recvedMessage.size (); ++i) {
				auto& str = pc.recvedMessage[i];
				auto& senderName = pc.senderName[i];
				auto& senderMAC = pc.senderMAC[i];
				QString tmp;
				tmp.append (ui.timeEdit->text ());
				tmp.append ("  ");
				tmp.append (QString::fromStdString (pc.name));
				tmp.append ("(" + QString::fromStdString (pc.MAC) + ")");
				tmp.append (QStringLiteral ("�յ�����"));
				tmp.append (QString::fromStdString (senderName));
				tmp.append ("(" + QString::fromStdString (senderMAC) + ")");
				tmp.append (QStringLiteral ("����Ϣ: "));
				tmp.append (QString::fromStdString (str));
				tmp.append ("\n");
				ui.messageBox->textCursor ().insertText (tmp);
			}
			pc.clearMessage ();

		}
	}
}

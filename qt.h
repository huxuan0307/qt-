#pragma once

#include <QtWidgets/QMainWindow>
#include <qtoolbutton>
#include <QStandardItemModel>
#include <QCombobox>
#include <QTimer>
#include "painter.h"
#include "x64/Debug/uic/ui_qt.h"
#include "TransparentBridge.h"
//#include "ui_qt.h"
using LinkProtocal::PC;
using TB = LinkProtocal::TransparentBridge;
class qt : public QMainWindow
{
	Q_OBJECT

public:
	qt(QWidget *parent = Q_NULLPTR);

private:
	Ui::qtClass ui;
	void readMACfromfile ();
	void init ();

private slots:
	void every50msdo ();

	void sendMessage_toolButtonclicked ();

	void showDetailInfoPC (QToolButton* qtb, PC&);

	void showDetailInfoTB (QToolButton* qtb, TB&, QTableView* tableView);

	void goAhead ();

	void nextStep ();

	void syncPC_MAC (QComboBox*, QLineEdit*);
	void showMessageOnStatusBar (const string& _Str, const size_t& _RemainTime) const;
	void updateMessageInMessageBox ();
	void checkPCRecvedMessage ();

private:
	string PCMAC0 = "11:11:11:11:11:11";
	string PCMAC1 = "22:22:22:22:22:22";
	string PCMAC2 = "33:33:33:33:33:33";
	string PCMAC3 = "44:44:44:44:44:44";
	string PCMAC4 = "55:55:55:55:55:55";
	string PCMAC5 = "66:66:66:66:66:66";
	Painter* paintpointer;
	QTimer* main_timer;
	QTimer* step_timer;
	QVector< QToolButton*> qtoolbuttonv;
	int cnt;
};

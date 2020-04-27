#include "LogMessage.h"

void MessageOutPut (QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QFile logfile;
    QTextStream outstream;
    logfile.setFileName ("./log.txt");
    logfile.open (QIODevice::WriteOnly | QIODevice::Append);
    outstream.setDevice (&logfile);
    outstream.setCodec (QTextCodec::codecForName ("gbk"));
    QString formatMsg = qFormatLogMessage (type, context, msg);

    outstream << formatMsg.toLocal8Bit ().constData () << endl;
}
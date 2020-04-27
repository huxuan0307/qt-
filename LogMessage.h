#pragma once

#include <QApplication>
#include <qtransform>
#include <QTranslator>
#include <QDateTime>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

void MessageOutPut (QtMsgType type, const QMessageLogContext& context, const QString& msg);
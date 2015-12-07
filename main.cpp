#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include "mainwindow.h"
#include <qglobal.h>

#if defined(Q_OS_WIN)
void myMessageOutput(QtMsgType type, const char *msg)
 {
     //in this function, you can write the message to any stream!
     switch (type) {
     case QtDebugMsg:
         fprintf(stderr, "Debug: %s\n", msg);
         break;
     case QtWarningMsg:
         fprintf(stderr, "Warning: %s\n", msg);
         break;
     case QtCriticalMsg:
         fprintf(stderr, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         fprintf(stderr, "Fatal: %s\n", msg);
         abort();
     }
 }
#endif

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    qInstallMsgHandler(myMessageOutput);
#endif
    QApplication a(argc, argv);

    QFont df;
    df = a.font();
    df.setPointSize(12);
    a.setFont(df);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}

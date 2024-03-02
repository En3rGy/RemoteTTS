#include "remotetts.h"
#include "systemstatus.h"
#include <QTextStream>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication::setApplicationName( "RemoteTTS");
    QApplication::setApplicationVersion( "0.1.6.0");
    QApplication::setOrganizationName( "paul-family");
    QApplication::setOrganizationDomain( "www.paul-family.de" );
    QApplication::setApplicationDisplayName( QApplication::applicationName() + " v" + QApplication::applicationVersion() );

    QTextStream(stdout) << QApplication::applicationDisplayName() << Qt::endl;

    CSystemStatus a(argc, argv);
    CNativeEventFilter grNativeEventFilter( & a );

    a.installNativeEventFilter( & grNativeEventFilter );

    RemoteTTS w;

    QStringList cmdline_args = QCoreApplication::arguments();
    for (const auto & arg : cmdline_args) {
        if(arg.toLower() == "--alive") {
            QTextStream(stdout) << "Reporting resumed... ";
            w.slot_resume();
            QTextStream(stdout) << "Done. Alive." << Qt::endl;
            return EXIT_SUCCESS;
        }
        else if (arg.toLower() =="--byebye") {
            QTextStream(stdout) << "Reporting suspending... ";
            w.slot_suspend();
            QTextStream(stdout) << "Done. Byebye." << Qt::endl;
            return EXIT_SUCCESS;
        }
    }


    QObject::connect( & a, SIGNAL( signal_goingToSuspend() ), & w, SLOT( slot_suspend() ) );
    QObject::connect( & a, SIGNAL( signal_systemResume() ), & w, SLOT( slot_resume() ) );

    w.show();

    return a.exec();
}

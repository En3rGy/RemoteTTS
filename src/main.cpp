#include "remotetts.h"
#include "systemstatus.h"

int main(int argc, char *argv[])
{
    QApplication::setApplicationName( "RemoteTTS");
    QApplication::setApplicationVersion( "0.1.1.0");
    QApplication::setOrganizationName( "paul-family");
    QApplication::setOrganizationDomain( "www.paul-family.de" );
    QApplication::setApplicationDisplayName( QApplication::applicationName() + " v" + QApplication::applicationVersion() );

    CSystemStatus a(argc, argv);
    CNativeEventFilter grNativeEventFilter( & a );

    a.installNativeEventFilter( & grNativeEventFilter );

    RemoteTTS w;

    QObject::connect( & a, SIGNAL( signal_goingToSuspend() ), & w, SLOT( slot_suspend() ) );
    QObject::connect( & a, SIGNAL( signal_systemResume() ), & w, SLOT( slot_resume() ) );

    w.show();

    return a.exec();
}

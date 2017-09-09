#ifndef REMOTETTS_H
#define REMOTETTS_H

#include <QMainWindow>
#include <QTimer>
#include <QSound>
#include <QTextToSpeech>
#include "udpmanager.h"
#include "tcpmanager.h"
#include "configform.h"

namespace Ui {
class RemoteTTS;
}

namespace IncommingMsgKeys
{
static const QString m_sKey_Play   = "PLAY";
static const QString m_sKey_Bell   = "BELL";
static const QString m_sKey_Say    = "SAY";
static const QString m_sKey_Captue = "CAPTURE";
}


class RemoteTTS : public QMainWindow
{
    Q_OBJECT

public:
    explicit RemoteTTS(QWidget *parent = 0);
    ~RemoteTTS();

public slots:
    void slot_msgReceived(QByteArray p_grMsg );
    void slot_resume( void );
    void slot_suspend( void );
    void slot_separationTimeOut( void );
    void slot_saySample( void );
    void slot_playIntro( void );
    void slot_processAudio( void );
    void slot_quitApp( void );

private:
    bool audioDeviceReady( void );

    QSound        * m_pSound;
    QTimer          m_grSepTimer;
    QTextToSpeech   m_grTTS;

    ConfigForm    * m_pConfigForm;

    Ui::RemoteTTS *ui;

    QStringList m_sSayList;
    QStringList m_sClipList;

    CUdpManager m_grUdpMgr;
    CTcpManager m_grTpcMgr;
};

#endif // REMOTETTS_H

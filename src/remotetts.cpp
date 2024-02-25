#include "remotetts.h"
#include "ui_remotetts.h"
#include <QTextToSpeech>
#include <QDebug>
#include <QVoice>
#include <QComboBox>
#include <QDir>
#include <QTime>

RemoteTTS::RemoteTTS(QWidget *parent) :
    QMainWindow(parent)
    , m_pSound( nullptr )
    , ui(new Ui::RemoteTTS)
{
    ui->setupUi(this);

    m_pConfigForm = new ConfigForm( this );
    ui->centralWidget->layout()->addWidget( m_pConfigForm );

    QSizePolicy grPolicy = m_pConfigForm->sizePolicy();
    grPolicy.setVerticalPolicy( QSizePolicy::MinimumExpanding );
    m_pConfigForm->setSizePolicy( grPolicy );

    QVector<QVoice> grVoices = m_grTTS.availableVoices();
    foreach( QVoice grVoice, grVoices ) {
        m_pConfigForm->getVoiceComboBox()->addItem( grVoice.name() );
    }

    m_pConfigForm->loadConfig();

    m_grTpcMgr.listen( m_pConfigForm->getValue( g_sConfig_ttsInPort ).toInt() );

    connect( m_pConfigForm, SIGNAL(signal_saySample()), this, SLOT( slot_saySample()) );
    connect( m_pConfigForm, SIGNAL( signal_playIntro() ), this, SLOT( slot_playIntro() )  );
    connect( m_pConfigForm, SIGNAL( signal_quitApp()), this, SLOT( slot_quitApp() ) );
    connect( & m_grTpcMgr, SIGNAL( signal_receivedMessage( QByteArray )), this, SLOT( slot_msgReceived(QByteArray)));

    QString sIp = m_pConfigForm->getValue( g_sConfig_receiverIP ).toString();
    int nPort = m_pConfigForm->getValue( g_sConfig_receiverPort ).toInt();
    m_pConfigForm->logEvent( tr( "Started, sending status to " ) + sIp  + ":" + QString::number( nPort) );
    m_grUdpMgr.sendData( "<ServerRunning>1</ServerRunning>", sIp, nPort );
}

RemoteTTS::~RemoteTTS()
{
    delete ui;
}

void RemoteTTS::slot_resume()
{
    qDebug() << Q_FUNC_INFO;

    QString sIp = m_pConfigForm->getValue( g_sConfig_receiverIP ).toString();
    int nPort = m_pConfigForm->getValue( g_sConfig_receiverPort ).toInt();
    m_pConfigForm->logEvent( tr( "Resuming, sending status to " ) + sIp  + ":" + QString::number( nPort) );
    m_grUdpMgr.sendData( "<ServerRunning>1</ServerRunning>", sIp, nPort );
}

void RemoteTTS::slot_suspend()
{
    qDebug() << Q_FUNC_INFO;

    QString sIp = m_pConfigForm->getValue( g_sConfig_receiverIP ).toString();
    int nPort = m_pConfigForm->getValue( g_sConfig_receiverPort ).toInt();
    m_pConfigForm->logEvent( tr( "Suspending, sending status to " )  + sIp  + ":" + QString::number( nPort) );
    m_grUdpMgr.sendData( "<ServerRunning>0</ServerRunning>", sIp, nPort );
}

void RemoteTTS::slot_separationTimeOut()
{
    if ( ! m_grSepTimer.isActive() ) {
        slot_processAudio();
    }
    else {
        qDebug() << "Timer still active" << Q_FUNC_INFO;
    }
}

void RemoteTTS::slot_saySample()
{
    m_sSayList.append( "1 2 3 4 5" );
    slot_processAudio();
}

void RemoteTTS::slot_playIntro() {
    QString sIntroFile = m_pConfigForm->getValue( g_sConfig_intoFile ).toString();

    if ( ! sIntroFile.isEmpty() ) {
        m_pConfigForm->logEvent( tr( "Playing intro file: " ) + sIntroFile );
        m_sClipList.prepend( sIntroFile );
    }

    if ( m_grSepTimer.isActive() == false ) {
        slot_separationTimeOut();
    }
}

void RemoteTTS::slot_msgReceived(QByteArray p_grMsg)
{
    QString sMsg = QString::fromLatin1( p_grMsg );
    QString sRequest = sMsg;

    qDebug() << sMsg << Q_FUNC_INFO;

    m_pConfigForm->logEvent( tr( "Received message: " ) + sMsg );

    if ( ( m_sSayList.isEmpty() ) && ( m_sClipList.isEmpty() ) ) {
        QString sIntroFile = m_pConfigForm->getValue( g_sConfig_intoFile ).toString();

         m_pConfigForm->logEvent( tr( "Intro file prepended " ) );

        if ( ! sIntroFile.isEmpty() )
            m_sClipList.prepend( sIntroFile );
    }

    try {
        if ( sMsg.startsWith( IncommingMsgKeys::m_sKey_Play, Qt::CaseInsensitive ) == true ) {
            sRequest.remove(0, 4 );
            m_sClipList.append( m_pConfigForm->getValue( g_sConfig_clipDir ).toString() + QDir::separator() + sRequest );
        }
        else if ( sMsg.startsWith( IncommingMsgKeys::m_sKey_Say, Qt::CaseInsensitive ) == true ) {
            sRequest.remove(0, 3 );
            /// @todo do xml decoding, e.g. &#228; --> ä
            sRequest = sRequest.replace( "&#228;", "ä" );
            sRequest = sRequest.replace( "&#176;", "°" );
            sRequest = sRequest.replace( "&#178;", "²" );
            sRequest = sRequest.replace( "&#179;", "³" );
            sRequest = sRequest.replace( "&#246;", "ö" );
            sRequest = sRequest.replace( "&#252;", "ü" );
            sRequest = sRequest.replace( "&#196;", "Ä" );
            sRequest = sRequest.replace( "&#214;", "Ö" );
            sRequest = sRequest.replace( "&#220;", "Ü" );
            sRequest = sRequest.replace( "&#223;", "ß" );
            m_sSayList.append( sRequest );
        }

        if ( m_grSepTimer.isActive() == false ) {
            slot_separationTimeOut();
        }
    }
    catch ( ... ) {
        qDebug() << tr( "Cought unknown exception, processing request: " ).toStdString().c_str() << sMsg;
    }
}

void RemoteTTS::slot_processAudio()
{
    int nAudioDelay_ms = m_pConfigForm->getValue( g_sConfig_sepTimer_ms ).toInt();

    // Process play of recorded sound files
    if (  m_sClipList.isEmpty() == false ) {
        if ( audioDeviceReady() == false ) {
            qDebug() << "audioDeviceReady() == false restarting timer" << Q_FUNC_INFO;
            m_grSepTimer.singleShot( nAudioDelay_ms, this, SLOT( slot_separationTimeOut()) ); /// @todo recheck starting point of timer
            return;
        }

        qDebug() << "m_sPlayList.isEmpty() == false" << Q_FUNC_INFO;

        if ( m_pSound == nullptr ) {
            QString sFile = QString("") + m_sClipList.first();
            qDebug() << "Playing" << sFile << Q_FUNC_INFO;

            m_pSound = new QSoundEffect();
            m_pSound->setSource(QUrl::fromLocalFile(sFile));
        }
        m_pSound->play();
        m_sClipList.removeFirst();

        qDebug() << "Finished starting playing, restarting timer" << Q_FUNC_INFO;
        m_grSepTimer.singleShot( nAudioDelay_ms, this, SLOT( slot_separationTimeOut()) );
        return;
    }

    // Process speech
    else if ( m_sSayList.isEmpty() == false ) {
        qDebug() << "m_sSayList.isEmpty() == false" << Q_FUNC_INFO;

        if ( audioDeviceReady() == false ) {
            m_grSepTimer.singleShot( nAudioDelay_ms, this, SLOT( slot_separationTimeOut()) );
            return;
        }

        try {
            QString sVoice = m_pConfigForm->getValue( g_sConfig_TTSVoice ).toString();
            QVoice grVoice = m_grTTS.voice();
            QVector< QVoice > grVoices = m_grTTS.availableVoices();
            if ( sVoice != grVoice.name() ) {
                foreach( QVoice grAvailAVoice, grVoices ) {
                    if ( sVoice == grAvailAVoice.name() ) {
                        m_grTTS.setVoice( grAvailAVoice );
                        break;
                    }
                }
            }

            qDebug() << "Telling: " << m_sSayList.first() << Q_FUNC_INFO;
            m_grTTS.say( m_sSayList.first() );
            qDebug() << "Tell commanded" << Q_FUNC_INFO;
            //m_bSpeechFinished = false;
            m_sSayList.removeFirst();
        }
        //        catch ( QTextToSpeech::Error p_QtSpeechError )
        //        {
        //            QLOG_ERROR() << "p_QtSpeechError.msg:" << p_QtSpeechError.msg << Q_FUNC_INFO;
        //        }
        catch ( ... ) {
            qWarning() << "Uncought exception" << Q_FUNC_INFO;
        }

        qDebug() << "Finished starting telling, restarting timer" << Q_FUNC_INFO;
        m_grSepTimer.singleShot( nAudioDelay_ms, this, SLOT( slot_separationTimeOut()) );
        return;
    }

    // within this execution path, everything is saied and played

    qDebug() << "Everything done, cleaning up..." << Q_FUNC_INFO;

    m_sSayList.clear();
    m_sClipList.clear();

    m_grSepTimer.stop();
}

void RemoteTTS::slot_quitApp()
{
    qDebug() << Q_FUNC_INFO;

    QString sIp = m_pConfigForm->getValue( g_sConfig_receiverIP ).toString();
    int nPort = m_pConfigForm->getValue( g_sConfig_receiverPort ).toInt();
    m_pConfigForm->logEvent( tr( "Closing down, sending status to " ) + sIp  + ":" + QString::number( nPort) );
    m_grUdpMgr.sendData( "<ServerRunning>0</ServerRunning>", sIp, nPort );

    QCoreApplication::quit();
}

bool RemoteTTS::audioDeviceReady()
{
    if (m_pSound) {
        if (m_pSound->isPlaying()) {
            return false;
        }
        else {
            delete m_pSound;
            m_pSound = nullptr;
        }
    }
    if (m_grTTS.state() == QTextToSpeech::Speaking) {
        return false;
    }

    return true;
}

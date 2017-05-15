#include "tcpmanager.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QVector>

CTcpManager::CTcpManager(QObject *parent) :
    QObject(parent)
  , m_pTcpSocket( nullptr )
  , m_pTcpServer( nullptr )
{
    m_pTcpServer = new QTcpServer( this );

    connect( m_pTcpServer, SIGNAL( newConnection()), SLOT( solt_newConnection()) );
}

CTcpManager::~CTcpManager()
{
    m_pTcpServer->close();
}

void CTcpManager::listen( const uint p_nPort )
{
    qDebug() << Q_FUNC_INFO;

    m_nPort = p_nPort;

    if ( m_pTcpServer->isListening() == true ) {
        m_pTcpServer->close();
    }

    m_pTcpServer->listen( QHostAddress::Any, m_nPort );
}

void CTcpManager::solt_newConnection()
{
    qDebug() << Q_FUNC_INFO;

    m_pTcpSocket = m_pTcpServer->nextPendingConnection();

//    bool bIpRegistered = false;

//    QHostAddress grHomeServerAddr( m_pModel->getString( StrVal::g_sHomeServerIP ) );

//    if ( m_pTcpSocket->peerAddress().toIPv4Address() == grHomeServerAddr.toIPv4Address() ) {
//        bIpRegistered = true;
//    }

//    if ( bIpRegistered == true ) {
        connect( m_pTcpSocket, SIGNAL(readyRead()), this, SLOT( slot_startRead() ) );
        connect( m_pTcpSocket, SIGNAL(disconnected()), this, SLOT( slot_disconnected() ) );
//    }
//    else
//    {
//        m_pTcpSocket->close();
//    }
}

void CTcpManager::slot_startRead()
{
    qDebug() << Q_FUNC_INFO;

    QByteArray grData = m_pTcpSocket->readAll();

    if ( grData.size() > 0 ) {
        m_grMsgData.append( grData );
    }
}

void CTcpManager::slot_disconnected()
{
    qDebug() << m_grMsgData << Q_FUNC_INFO;

    emit signal_receivedMessage( m_grMsgData );
    m_grMsgData.clear();
}

#include "udpmanager.h"
#include <QUdpSocket>
#include <QVector>

CUdpManager::CUdpManager(QObject *parent) :
    QObject(parent)
  , m_pUdpSocket( nullptr )
{
}

void CUdpManager::listen( const uint p_nPort )
{
    m_nPort = p_nPort;

    if ( m_pUdpSocket != nullptr ) {
        delete m_pUdpSocket;
        m_pUdpSocket = nullptr;
    }

    m_pUdpSocket = new QUdpSocket();

    connect( m_pUdpSocket, SIGNAL( readyRead()), this, SLOT( solt_readPendingDatagrams()));
    m_pUdpSocket->bind( p_nPort );
}

void CUdpManager::solt_readPendingDatagrams()
{
    while ( m_pUdpSocket->hasPendingDatagrams() ) {
        QByteArray grData;
        grData.resize (m_pUdpSocket->pendingDatagramSize() );
        QHostAddress grSenderAddress;
        quint16 unSenderPort;

        m_pUdpSocket->readDatagram( grData.data(), grData.size(), & grSenderAddress, & unSenderPort);
        if ( grData.size() > 0 ) {
            QString sString = QString::fromLatin1( grData.data() );
            emit signal_receivedMessage( sString );
        }
        else {

        }
    }
}

void CUdpManager::sendData( const QString & p_sData, const QString & p_sHostAdress, const quint16 p_unPort )
{
    QByteArray   grByteArry;
    grByteArry.append(p_sData.toLatin1());
    QHostAddress grHostAdress(p_sHostAdress);
    QUdpSocket grUdpSocket;

    grUdpSocket.writeDatagram( grByteArry, grHostAdress, p_unPort );
}

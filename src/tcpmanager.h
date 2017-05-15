#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QString>

class QTcpSocket;
class QTcpServer;

class CTcpManager : public QObject
{
    Q_OBJECT
public:
    CTcpManager(QObject *parent = 0);
    ~CTcpManager();

    void listen( const uint p_nPort );

signals:
    void signal_receivedMessage( QByteArray );

public slots:
    void solt_newConnection( void );
    void slot_startRead( void );
    void slot_disconnected( void );

private:
    QTcpSocket * m_pTcpSocket;
    QTcpServer * m_pTcpServer;
    qint16       m_nPort;
    QByteArray   m_grMsgData;
};

#endif // UDPMANAGER_H

#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
#include <QTcpSocket>

class ServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit ServerWorker(QObject *parent = nullptr);
    virtual bool setSocketDescriptor(qintptr socketDescriptor);
    QString userName();
    void setUserName(QString user);

signals:
    void logMessage(const QString &Msg);
    void jsonReceived(ServerWorker *sender, const QJsonObject &docObj);

private:
    QTcpSocket *m_serverSocket;
    QString m_userName;

public slots:
    void onReadyRead();
    void sendMessage(const QString &text, const QString &type = "message");
    void sendJson(const QJsonObject &json);
};

#endif // SERVERWORKER_H

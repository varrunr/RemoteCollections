
#include<QTcpServer>
#include<QObject>
#include<QHostAddress>

class HttpDaemon : public QTcpServer
{
     Q_OBJECT
     
public:
     HttpDaemon(quint16 port, QObject* parent = 0)
     :QTcpServer(parent), disabled(false)
	{
 		listen(QHostAddress::Any, port);
	}
     
     void incomingConnection(int socket);
     
     void pause();

     void resume();

private slots:
 
     void readClient();
     void discardClient();
     
private:
     bool disabled;
};

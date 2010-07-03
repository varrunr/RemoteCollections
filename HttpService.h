#include "HttpDaemon.h"
#include<QCoreApplication>
#include "App.h"

class HttpService 
{

public:

	HttpService(quint16 port, QCoreApplication *app);

protected:
	
	void setPort(quint16 port);
	
	void setApp(QCoreApplication *app);
	
	void start();
     
	void pause();

	void resume();
     
private:
     
	quint16 port;
     
	HttpDaemon *daemon;
     
	QCoreApplication *app;
     
};
 
 
 
 

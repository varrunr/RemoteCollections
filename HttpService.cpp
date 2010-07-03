
#include "HttpService.h"
#include "core/support/Debug.h"

HttpService::HttpService(quint16 port, QCoreApplication *app)
{
	setPort(port);
	setApp(app);
	start();
}

void HttpService::setPort(quint16 port)
{
	this->port = port;
}

void HttpService::setApp(QCoreApplication *app)
{
	this->app = app;
}

void 
HttpService::start()
{
         daemon = new HttpDaemon(port, app);

         if (!daemon->isListening()) {
             debug() << "Failed" ; //QString("Failed to bind to port %1").arg(daemon->serverPort());
         }
}
     
void 
HttpService::pause()
{
         daemon->pause();
}

void 
HttpService::resume()
{
         daemon->resume();
}



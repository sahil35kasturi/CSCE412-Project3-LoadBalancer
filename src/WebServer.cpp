#include "../include/WebServer.h"

WebServer::WebServer(int id)
    : id(id), busy(false), timeRemaining(0) {}

bool WebServer::isBusy() const
{
    return busy;
}

void WebServer::assignRequest(const Request &request)
{
    busy = true;
    timeRemaining = request.timeRequired;
}

void WebServer::tick()
{
    if (busy)
    {
        timeRemaining--;
        if (timeRemaining <= 0)
        {
            busy = false;
        }
    }
}

int WebServer::getTimeRemaining() const
{
    return timeRemaining;
}
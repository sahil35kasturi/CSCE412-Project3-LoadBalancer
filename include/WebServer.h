#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @brief Represents a web server that processes requests.
 */
class WebServer
{
private:
    int id;
    bool busy;
    int timeRemaining;

public:
    WebServer(int id);

    bool isBusy() const;
    void assignRequest(const Request &request);
    void tick();
    int getTimeRemaining() const;
    int getId() const;
};

#endif
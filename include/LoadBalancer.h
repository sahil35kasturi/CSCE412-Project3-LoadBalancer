#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <vector>
#include <string>
#include "WebServer.h"
#include "Request.h"

/**
 * @brief Manages web servers and request queue.
 */
class LoadBalancer
{
private:
    std::queue<Request> requestQueue;
    std::vector<WebServer> servers;

    int totalProcessed;
    int totalDropped;
    int totalGenerated;
    int scaleUpCount;
    int scaleDownCount;
    int cooldown;

    int minTaskTime;
    int maxTaskTime;

    std::vector<std::string> blockedIPs;

public:
    LoadBalancer(int initialServers, int minTime, int maxTime);

    void generateInitialQueue();
    void simulate(int cycles);
    void addRequest();
    bool isBlocked(const std::string &ip);
    void assignRequests();
    void tickServers();
    void checkScaling();
    void printSummary(int startingQueueSize);
};

#endif
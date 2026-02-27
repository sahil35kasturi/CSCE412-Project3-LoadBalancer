#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include "WebServer.h"
#include "Request.h"

/**
 * @brief Manages web servers, request queue, scaling, and statistics.
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

    // Statistics tracking
    int peakQueueSize;
    int peakServers;
    int minServers;

    std::vector<std::string> blockedIPs;

public:
    LoadBalancer(int initialServers, int minTime, int maxTime);

    void generateInitialQueue();
    void simulate(int cycles);

    void addRequest();
    bool isBlocked(const std::string &ip);

    void assignRequests();
    void tickServers();

    void checkScaling(std::ofstream &logFile);

    int countActiveServers() const;

    void printSummary(int startingQueueSize);
};

#endif
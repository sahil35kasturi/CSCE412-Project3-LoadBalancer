#include "../include/LoadBalancer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

LoadBalancer::LoadBalancer(int initialServers, int minTime, int maxTime)
    : totalProcessed(0), totalDropped(0), totalGenerated(0),
      scaleUpCount(0), scaleDownCount(0), cooldown(0),
      minTaskTime(minTime), maxTaskTime(maxTime)
{

    for (int i = 0; i < initialServers; i++)
    {
        servers.emplace_back(i);
    }

    blockedIPs.push_back("192.168.1.");
}

void LoadBalancer::generateInitialQueue()
{
    int initialSize = servers.size() * 100;
    for (int i = 0; i < initialSize; i++)
    {
        addRequest();
    }
}

void LoadBalancer::simulate(int cycles)
{
    int startingQueueSize = requestQueue.size();

    for (int i = 0; i < cycles; i++)
    {

        if (rand() % 3 == 0)
        {
            addRequest();
        }

        assignRequests();
        tickServers();

        if (cooldown > 0)
        {
            cooldown--;
        }
        else
        {
            checkScaling();
        }
    }

    printSummary(startingQueueSize);
}

void LoadBalancer::addRequest()
{
    std::string ip = std::to_string(rand() % 255) + "." +
                     std::to_string(rand() % 255) + "." +
                     std::to_string(rand() % 255) + "." +
                     std::to_string(rand() % 255);

    if (isBlocked(ip))
    {
        totalDropped++;
        return;
    }

    int time = minTaskTime + rand() % (maxTaskTime - minTaskTime + 1);
    char type = (rand() % 2 == 0) ? 'P' : 'S';

    requestQueue.emplace(ip, "10.0.0.1", time, type);
    totalGenerated++;
}

bool LoadBalancer::isBlocked(const std::string &ip)
{
    for (const auto &blocked : blockedIPs)
    {
        if (ip.find(blocked) == 0)
        {
            return true;
        }
    }
    return false;
}

void LoadBalancer::assignRequests()
{
    for (auto &server : servers)
    {
        if (!server.isBusy() && !requestQueue.empty())
        {
            server.assignRequest(requestQueue.front());
            requestQueue.pop();
        }
    }
}

void LoadBalancer::tickServers()
{
    for (auto &server : servers)
    {
        if (server.isBusy())
        {
            server.tick();
            if (!server.isBusy())
            {
                totalProcessed++;
            }
        }
    }
}

void LoadBalancer::checkScaling()
{
    int currentSize = requestQueue.size();
    int serverCount = servers.size();

    if (currentSize > 80 * serverCount)
    {
        servers.emplace_back(serverCount);
        scaleUpCount++;
        cooldown = 50;
    }

    else if (currentSize < 50 * serverCount && serverCount > 1)
    {
        servers.pop_back();
        scaleDownCount++;
        cooldown = 50;
    }
}

void LoadBalancer::printSummary(int startingQueueSize)
{
    std::cout << "\n--- Load Balancer Summary ---\n";
    std::cout << "Starting Queue Size: " << startingQueueSize << "\n";
    std::cout << "Ending Queue Size: " << requestQueue.size() << "\n";
    std::cout << "Task Time Range: " << minTaskTime << " - " << maxTaskTime << "\n";
    std::cout << "Total Requests Generated: " << totalGenerated << "\n";
    std::cout << "Total Requests Processed: " << totalProcessed << "\n";
    std::cout << "Total Requests Dropped: " << totalDropped << "\n";
    std::cout << "Scale Ups: " << scaleUpCount << "\n";
    std::cout << "Scale Downs: " << scaleDownCount << "\n";
    std::cout << "Active Servers: " << servers.size() << "\n";
}
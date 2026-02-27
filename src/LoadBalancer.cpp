#include "../include/LoadBalancer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

LoadBalancer::LoadBalancer(int initialServers, int minTime, int maxTime)
    : totalProcessed(0),
      totalDropped(0),
      totalGenerated(0),
      scaleUpCount(0),
      scaleDownCount(0),
      cooldown(0),
      minTaskTime(minTime),
      maxTaskTime(maxTime),
      peakQueueSize(0),
      peakServers(initialServers),
      minServers(initialServers)
{
    for (int i = 0; i < initialServers; i++)
    {
        servers.emplace_back(i);
    }

    // Example blocked IP range
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

    std::ofstream logFile("logs/loadbalancer_log.txt");

    logFile << "LOAD BALANCER SIMULATION LOG\n";
    logFile << "=========================================\n";
    logFile << "Initial Server Count: " << servers.size() << "\n";
    logFile << "Simulation Cycles: " << cycles << "\n";
    logFile << "Initial Queue Size: " << startingQueueSize << "\n";
    logFile << "Task Time Range: " << minTaskTime << " - " << maxTaskTime << "\n\n";

    for (int cycle = 0; cycle < cycles; cycle++)
    {
        // Random new request generation
        if (rand() % 3 == 0)
        {
            addRequest();
        }

        assignRequests();
        tickServers();

        // Track peak queue
        if (requestQueue.size() > peakQueueSize)
            peakQueueSize = requestQueue.size();

        // Track server extremes
        if (servers.size() > peakServers)
            peakServers = servers.size();

        if (servers.size() < minServers)
            minServers = servers.size();

        // Activity tracking every 500 cycles
        if (cycle % 500 == 0)
        {
            logFile << "[Activity] Cycle " << cycle
                    << " | Generated: " << totalGenerated
                    << " | Processed: " << totalProcessed
                    << " | Dropped: " << totalDropped
                    << "\n";
        }

        // Scaling logic
        if (cooldown > 0)
        {
            cooldown--;
        }
        else
        {
            checkScaling(logFile);
        }

        // Periodic state snapshot every 1000 cycles
        if (cycle % 1000 == 0)
        {
            logFile << "[Cycle " << cycle << "] "
                    << "Queue: " << requestQueue.size()
                    << " | Servers: " << servers.size()
                    << " | Active: " << countActiveServers()
                    << "\n";
        }
    }

    // Final summary written to file
    logFile << "\n================ FINAL SUMMARY ================\n";
    logFile << "Starting Queue Size: " << startingQueueSize << "\n";
    logFile << "Ending Queue Size: " << requestQueue.size() << "\n";
    logFile << "Total Requests Generated: " << totalGenerated << "\n";
    logFile << "Total Requests Processed: " << totalProcessed << "\n";
    logFile << "Total Requests Dropped: " << totalDropped << "\n";
    logFile << "Scale Ups: " << scaleUpCount << "\n";
    logFile << "Scale Downs: " << scaleDownCount << "\n";
    logFile << "Final Server Count: " << servers.size() << "\n";
    logFile << "Active Servers: " << countActiveServers() << "\n";
    logFile << "Inactive Servers: " << servers.size() - countActiveServers() << "\n";
    logFile << "Peak Queue Size: " << peakQueueSize << "\n";
    logFile << "Peak Servers: " << peakServers << "\n";
    logFile << "Minimum Servers: " << minServers << "\n";
    logFile << "===============================================\n";

    logFile.close();

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

void LoadBalancer::checkScaling(std::ofstream &logFile)
{
    int currentSize = requestQueue.size();
    int serverCount = servers.size();

    if (currentSize > 80 * serverCount)
    {
        logFile << "[SCALE UP] Queue: "
                << currentSize
                << " Servers: "
                << serverCount
                << "\n";

        servers.emplace_back(serverCount);
        scaleUpCount++;
        cooldown = 50;
    }
    else if (currentSize < 50 * serverCount && serverCount > 1)
    {
        logFile << "[SCALE DOWN] Queue: "
                << currentSize
                << " Servers: "
                << serverCount
                << "\n";

        servers.pop_back();
        scaleDownCount++;
        cooldown = 50;
    }
}

int LoadBalancer::countActiveServers() const
{
    int count = 0;
    for (const auto &server : servers)
    {
        if (server.isBusy())
            count++;
    }
    return count;
}

void LoadBalancer::printSummary(int startingQueueSize)
{
    std::cout << "\n========== FINAL SUMMARY ==========\n";
    std::cout << "Starting Queue Size: " << startingQueueSize << "\n";
    std::cout << "Ending Queue Size: " << requestQueue.size() << "\n";
    std::cout << "Task Time Range: " << minTaskTime << " - " << maxTaskTime << "\n";
    std::cout << "Total Requests Generated: " << totalGenerated << "\n";
    std::cout << "Total Requests Processed: " << totalProcessed << "\n";
    std::cout << "Total Requests Dropped: " << totalDropped << "\n";
    std::cout << "Scale Ups: " << scaleUpCount << "\n";
    std::cout << "Scale Downs: " << scaleDownCount << "\n";
    std::cout << "Final Server Count: " << servers.size() << "\n";
    std::cout << "Active Servers: " << countActiveServers() << "\n";
    std::cout << "Inactive Servers: " << servers.size() - countActiveServers() << "\n";
    std::cout << "Peak Queue Size: " << peakQueueSize << "\n";
    std::cout << "Peak Servers: " << peakServers << "\n";
    std::cout << "Minimum Servers: " << minServers << "\n";
    std::cout << "====================================\n";
}
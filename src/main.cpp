/**
 * @mainpage CSCE 412 Project 3 - Load Balancer Simulation
 *
 * @section overview Overview
 *
 * This project simulates a dynamic load balancer system that distributes
 * incoming web requests across multiple web servers. The simulation models
 * request generation, processing, queue management, and dynamic server scaling.
 *
 * The load balancer monitors the request queue and automatically scales
 * the number of active web servers up or down depending on system load.
 *
 * @section components System Components
 *
 * - LoadBalancer: Manages the request queue and server scaling logic.
 * - WebServer: Processes incoming requests and tracks server workload.
 * - Request: Represents an individual web request with a processing time.
 *
 * @section functionality Key Features
 *
 * - Random request generation
 * - Queue tracking and statistics
 * - Automatic server scaling
 * - Simulation summary metrics
 *
 * This documentation was generated using Doxygen.
 */

#include "../include/LoadBalancer.h"
#include <iostream>
#include <ctime>

int main()
{
    srand(time(0));

    int servers;
    int cycles;

    std::cout << "Enter number of web servers: ";
    std::cin >> servers;

    std::cout << "Enter simulation clock cycles: ";
    std::cin >> cycles;

    LoadBalancer lb(servers, 2, 15);

    lb.generateInitialQueue();
    lb.simulate(cycles);

    return 0;
}
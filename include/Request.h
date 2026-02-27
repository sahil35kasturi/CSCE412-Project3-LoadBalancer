#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @brief Represents a single web request in the load balancer system.
 */
struct Request
{
    std::string ipIn;
    std::string ipOut;
    int timeRequired;
    char jobType; // 'P' or 'S'

    Request(std::string in, std::string out, int time, char type);
};

#endif
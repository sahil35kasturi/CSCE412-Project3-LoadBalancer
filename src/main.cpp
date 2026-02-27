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
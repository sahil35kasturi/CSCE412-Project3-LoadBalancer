#include "../include/Request.h"

Request::Request(std::string in, std::string out, int time, char type)
    : ipIn(in), ipOut(out), timeRequired(time), jobType(type) {}
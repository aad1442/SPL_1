//network_functions.cpp
//#include "network_functions.h"
#include <cstdlib>
#include <iostream>


void list_network_connections()
{
    system("netstat -tunap | grep ESTABLISHED");
}
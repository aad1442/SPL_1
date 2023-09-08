#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Structure to store network connection information
struct NetworkConnection
{
    std::string protocol;
    std::string localAddress;
    std::string remoteAddress;
    std::string state;
};

// Function to format IP addresses from hexadecimal to "192.168.10.102:53748" format
std::string format_ip_address(const std::string &hex_ip, unsigned int hex_port)
{
    // Convert hexadecimal IP address to decimal
    unsigned int ip_parts[4];
    sscanf(hex_ip.c_str(), "%2x%2x%2x%2x", &ip_parts[0], &ip_parts[1], &ip_parts[2], &ip_parts[3]);

    // Format the IP address and port
    std::ostringstream formatted;
    formatted << ip_parts[3] << "." << ip_parts[2] << "." << ip_parts[1] << "." << ip_parts[0] << ":" << hex_port;
    return formatted.str();
}

// Function to parse network connections
std::vector<NetworkConnection> parse_network_connections()
{
    std::vector<NetworkConnection> connections;
    std::ifstream netstat_output("/proc/net/tcp");

    if (!netstat_output)
    {
        std::cerr << "Failed to open /proc/net/tcp" << std::endl;
        return connections;
    }

    // Skip the header line
    std::string line;
    std::getline(netstat_output, line);

    while (std::getline(netstat_output, line))
    {
        std::istringstream iss(line);
        NetworkConnection connection;
        std::string protocol, local, remote, state;

        // Extract protocol, local and remote addresses, and connection state
        iss >> protocol >> local >> remote >> state;

        connection.protocol = protocol;

        // Declare the ip_parts array here
        unsigned int ip_parts[4];
        unsigned int localPort, remotePort;
        
        // Extract the port numbers in hexadecimal and convert them to decimal
        sscanf(local.c_str(), "%x:%x", &ip_parts[0], &localPort);
        sscanf(remote.c_str(), "%x:%x", &ip_parts[0], &remotePort);

        // Format local and remote addresses with port numbers
        connection.localAddress = format_ip_address(local, localPort);
        connection.remoteAddress = format_ip_address(remote, remotePort);
        connection.state = state;

        // Filter established connections
        if (state == "01")
        { // 01 corresponds to ESTABLISHED state
            connections.push_back(connection);
        }
    }

    netstat_output.close();
    return connections;
}

// Function to log network connections to a file
void log_network_connections(const std::vector<NetworkConnection> &connections, const std::string &filename)
{
    std::ofstream logfile(filename);

    if (!logfile)
    {
        std::cerr << "Failed to open the log file for writing" << std::endl;
        return;
    }

    logfile << "Network Connections:" << std::endl;
    for (const NetworkConnection &connection : connections)
    {
        // logfile << "Protocol: " << connection.protocol << std::endl;
        logfile << "Local Address: " << connection.localAddress << std::endl;
        logfile << "Remote Address: " << connection.remoteAddress << std::endl;
        logfile << "State: " << connection.state << std::endl
                << std::endl;
    }

    logfile.close();
    std::cout << "Network connections logged to " << filename << std::endl;
}

int main()
{
    const std::string filename = "network_connections_log.txt";

    // Parse network connections
    std::vector<NetworkConnection> currentConnections = parse_network_connections();

    // Log network connections to the file
    log_network_connections(currentConnections, filename);

    return 0;
}

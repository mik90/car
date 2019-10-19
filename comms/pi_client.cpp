#include <iostream>
#include <cstring>
#include <cerrno>

#include <unistd.h>

#include "pi_client.hpp"

namespace Car
{

/** @brief Initializes socket **/
PiClient::PiClient()
{
    const int FLAGS = 0x0;
    // Init to failed state
    m_acceptedSocketFd = -1;

    m_socketFd = socket(AF_INET, SOCK_STREAM, FLAGS);
    if (m_socketFd < 0)
    {
        std::cerr << "socket() failure. errorno: " << std::strerror(errno)
                  << std::endl;
        return;
    }

    // Set up server struct 
    m_serverAddr.sin_family      = AF_INET;
    m_serverAddr.sin_addr.s_addr = INADDR_ANY;
    m_serverAddr.sin_port        = htons(ControlPortNumber);

    int status = bind(m_socketFd, (struct sockaddr*) &m_serverAddr,
                      sizeof(m_serverAddr));
    if (status < 0)
    {
        close(m_socketFd);
        std::cerr << "bind() failure. errorno: " << std::strerror(errno)
                  << std::endl;
        return;
    }
}



void PiClient::waitForConnection()
{
    // Attempt request 5 times
    listen(m_socketFd, 5);
    std::cout << "Waiting to connect..." << std::endl;
    socklen_t clientAddrLen = 0;
    int acceptedSocketFd = accept(m_socketFd,
                                  (struct sockaddr*)&m_clientAddr,
                                  &clientAddrLen);
    if (acceptedSocketFd < 0)
    {
        close(m_socketFd);
        std::cerr << "accept() failure. errorno: " << std::strerror(errno)
                    << std::endl;
        return;
    }
    std::cout << "Connection successful.\n";
}


} // End of namespace
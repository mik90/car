#include <iostream>
#include <cstring>
#include <cerrno>

#include <unistd.h>

#include "server.hpp"

namespace Car
{

/** @brief Initializes socket **/
Server::Server()
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


} // End of namespace
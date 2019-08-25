#include <iostream>
#include <cstring>

#include <unistd.h>
#include <errno.h>

#include "comms.hpp"

namespace Car
{

/** @brief Initializes socket **/
Comms::Comms()
{
    const int FLAGS = 0x0;

    m_socket_fd = socket(AF_INET, SOCK_STREAM, FLAGS);
    if (m_socket_fd < 0)
    {
        std::cerr << "socket() failure. errorno: " << std::strerror(errno)
                  << std::endl;
        return;
    }

    // Set up server struct 
    m_server_addr.sin_family      = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port        = htons(ControlPortNumber);

    int status = bind(m_socket_fd, (struct sockaddr*) &m_server_addr,
                      sizeof(m_server_addr));
    if (status < 0)
    {
        close(m_socket_fd);
        std::cerr << "bind() failure. errorno: " << std::strerror(errno)
                  << std::endl;
        return;
    }
}


}
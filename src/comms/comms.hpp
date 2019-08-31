#ifndef COMMS_HPP_
#define COMMS_HPP_

#include <netinet/in.h>

namespace Car
{

/** @brief Manages communications between user and car **/
class Comms
{
    private:
        struct sockaddr_in m_server_addr;
        struct sockaddr_in m_client_addr;
        int m_socket_fd;
    public:
        /** @brief This is the port that the car will be
         * getting instructions from */
        static const int ControlPortNumber = 2001;
        Comms();

};

}

#endif
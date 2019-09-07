#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <netinet/in.h>

#include "message.hpp"

namespace Car
{
/** @brief Manages communications between user and car **/
class Server
{
    private:
        struct sockaddr_in m_serverAddr;
        struct sockaddr_in m_clientAddr;
        int m_socketFd;
        int m_acceptedSocketFd;
    public:
        /** @brief This is the port that the car will be
         * getting instructions from */
        static const int ControlPortNumber = 2001;
        void waitForConnection();
        void sendCommand();
        Server();

};


}

#endif
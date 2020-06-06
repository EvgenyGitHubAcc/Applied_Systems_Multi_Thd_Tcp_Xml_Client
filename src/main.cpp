#include <winsock.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include "network.h"
#include "xml.h"

#define BUF_SIZE 64

int main(void)
{
    std::list<std::string> commList;

    if(!readXmlFile("ClientXmlFile.xml", commList))
    {
        exit(0);
    }

    SOCKET conn_socket;

    if (initConnection(&conn_socket))
    {
        return -1;
    }

    char buffer[BUF_SIZE] = "";

    while (true)
    {
        for(std::list<std::string>::iterator it = commList.begin(); it != commList.end(); ++it)
        {
            if(sendMsg(&conn_socket, it->data(), it->size()) == SOCKET_ERROR)
            {
                try
                {
                    reconnect(&conn_socket);
                }
                catch (std::string str)
                {
                    std::cerr << "Client: " + str << std::endl;
                    exit(-1);
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            memset(buffer, 0, BUF_SIZE);
            recvMsg(&conn_socket, buffer, sizeof(buffer));
        }
    }
    closeConnection(&conn_socket);
    system("pause");
    return 0;
}

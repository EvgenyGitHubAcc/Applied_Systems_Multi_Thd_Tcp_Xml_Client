#include "network.h"

int initConnection(SOCKET * p_conn_socket)
{
    char server_name[] = "localhost";
    unsigned short port = DEFAULT_PORT;
    unsigned int addr;
    int socket_type;
    struct sockaddr_in server;
    WSADATA wsaData;
    struct hostent* hp;

    socket_type = SOCK_STREAM;   // TCP-protocol

    if (WSAStartup(0x101, &wsaData) == SOCKET_ERROR)
    {
        std::cerr << "Client: WSAStartup failed with error" + std::to_string(WSAGetLastError()) << std::endl;
        WSACleanup();
        return -1;
    }

    if (isalpha(server_name[0]))
    {   /* server address is a name */
        hp = gethostbyname(server_name);
    }
    else
    { /* Convert nnn.nnn address to a usable one */
        addr = inet_addr(server_name);
        hp = gethostbyaddr((char*)& addr, 4, AF_INET);
    }
    if (hp == NULL)
    {
        std::cerr << "Client: Cannot resolve address [" + std::string(server_name) + "]. Error" + std::to_string(WSAGetLastError()) << std::endl;
        WSACleanup();
        exit(1);
    }
    // Create a socket
    *p_conn_socket = socket(AF_INET, socket_type, 0);
    if (*p_conn_socket == INVALID_SOCKET)
    {
        std::cerr << "Client: Error Opening socket. Error " + std::to_string(WSAGetLastError()) << std::endl;
        WSACleanup();
        return -1;
    }

    // Copy into the sockaddr_in structure

    memset(&server, 0, sizeof(server));
    memcpy(&(server.sin_addr), hp->h_addr, hp->h_length);
    server.sin_family = hp->h_addrtype;
    server.sin_port = DEFAULT_PORT;

    std::cout << "Client: Connecting to: " +  std::string(hp->h_name) << std::endl;
    if (connect(*p_conn_socket, (struct sockaddr*) & server, sizeof(server)) == SOCKET_ERROR)
    {
        std::cerr << "Client: Connect to " + std::string(hp->h_name) + " failed. Error " + std::to_string(WSAGetLastError()) << std::endl;
        WSACleanup();
        return -1;
    }
    return 0;
}

bool closeConnection(SOCKET* p_conn_socket)
{
    closesocket(*p_conn_socket);
    WSACleanup();
    return 0;
}

int sendMsg(SOCKET* p_conn_socket, const char* msg, int sizeof_msg)
{
    int retval = send(*p_conn_socket, msg, sizeof_msg, 0);
    if (retval == SOCKET_ERROR)
    {
        std::cerr << "Client: Send failed. Error " + std::to_string(WSAGetLastError()) << std::endl;
        closeConnection(p_conn_socket);
        return -1;
    }

    std::cout << "CLIENT: " + std::string(msg) << std::endl;

    return 0;
}

int recvMsg(SOCKET* p_conn_socket, char* msg, int sizeof_msg)
{
    int retval = recv(*p_conn_socket, msg, sizeof_msg, 0);
    if (retval == SOCKET_ERROR)
    {
        std::cerr << "Client: Recv failed. Error " + std::to_string(WSAGetLastError()) << std::endl;
        closeConnection(p_conn_socket);
        return -1;
    }

    if (retval == 0)
    {
        std::cout << "Server closed connection" << std::endl;
        closeConnection(p_conn_socket);
        return -1;
    }
    std::cout << "SERVER: " + std::string(msg) << std::endl;

    return 0;
}

void reconnect(SOCKET * connSocket)
{
    for(unsigned int i = RECONN_COUNT; i > 0; --i)
    {
        closeConnection(connSocket);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if(initConnection(connSocket) == SOCKET_ERROR)
        {
            continue;
        }
        else
        {
            return;
        }
    }
    throw std::string("Server can't be connected!");
}

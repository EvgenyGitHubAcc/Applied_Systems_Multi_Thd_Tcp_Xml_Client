#ifndef NETWORK_H
#define NETWORK_H

#define DEFAULT_PORT 1234
#define RECONN_COUNT 10

#include <winsock.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <exception>

int initConnection(SOCKET * p_conn_socket);
bool closeConnection(SOCKET* p_conn_socket);
int sendMsg(SOCKET* p_conn_socket, const char* msg, int sizeof_msg);
int recvMsg(SOCKET* p_conn_socket, char* msg, int sizeof_msg);
void reconnect(SOCKET * connSocket);

#endif // NETWORK_H

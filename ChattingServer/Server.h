#pragma once
#pragma warning(disable:4996)
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <format>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "Define.h"
#include "Server.h"
#include "USER.h"
#include "Manager.h"

class Server
{
private:
	Manager* manager;
	WSADATA	wsaData;
	SOCKET servSock, clntSock;
	SOCKADDR_IN servAddr, clntAddr;

	//SELECT ���ȯ�� ����
	TIMEVAL timeout;
	fd_set reads, cpyReads;
	int adrSz;
	int strLen, result;

	//�Է� ����
	SOCKET* targetSocket;
	char c;

public:
	Server();
	void PrepareServer();
	void StartConn();
	void EndConn();
	vector<string> Split(const string& str, char Delimiter);
};

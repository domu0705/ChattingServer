#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include "USER.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include <map>

using namespace std;

class Manager
{
private:
	map<SOCKET, User> userSockAry;
	//socket과 client 묶을 자료형

public:
	Manager();
	User GetUserFromSock(SOCKET sockNum);
	void SetUserToUserSockAry(SOCKET sockNum,User user);
};

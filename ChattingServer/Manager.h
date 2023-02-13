#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include "USER.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include <map>

using namespace std;

enum Command
{
	H,
	US,
	LT,
	ST,
	PF,
	TO,
	O,
	J,
	X
};

class Manager
{
private:
	//socket과 client 묶을 자료형

public:
	map<SOCKET, User> userAry;

	Manager();
	User GetUserFromSock(SOCKET sockNum);
	void SetUserToUserSockAry(SOCKET sockNum,User user);
};

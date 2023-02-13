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
public:
	map<SOCKET, User> userAry;//socket과 client 묶을 자료형
	map<string, User> nameAry;

	Manager();
	User GetUserFromSock(SOCKET sockNum);
	void SetUserToUserSockAry(SOCKET sockNum,User user);

	void LogIn(SOCKET sockNum, string id);
	void H(SOCKET sockNum);
	void US(SOCKET sockNum);
	void LT(SOCKET sockNum);
	void ST(SOCKET sockNum);
	void PF(SOCKET sockNum);
	void TO(SOCKET sockNum);
	void O(SOCKET sockNum);
	void J(SOCKET sockNum);
	void X(SOCKET sockNum);
	void NotExistingCommend(SOCKET sockNum);
};

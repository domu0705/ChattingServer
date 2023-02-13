// -----------------------------------------------------------------------------------
//  사용자의 상태 관리
// -----------------------------------------------------------------------------------

#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>

using namespace std;

enum State
{
	WAITING,
	LOBBY,
	ROOM
};

class User
{
private:
	SOCKET socketNum;
	State state;
	string ID;

public:
	User();
	User(SOCKET socketNum);

	State GetState();
	void SetState(State newState);
	string GetID();
	void SetID(string ID);

};

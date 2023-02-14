// -----------------------------------------------------------------------------------
//  클라이언트 정보
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
	string roomName;

public:
	User();
	User(SOCKET socketNum);

	State GetState();
	void SetState(State newState);
	const string GetID();
	void SetID(string ID);
	void SetRoom(string roomName);

};

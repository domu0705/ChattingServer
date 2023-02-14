// -----------------------------------------------------------------------------------
//  Ŭ���̾�Ʈ ����
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
	string ipAddr;
	State state;
	string id;
	string roomName;
	string roomInTime;//�濡 ������ �ð�

public:
	User();
	User(SOCKET socketNum, string ipAddr);

	State GetState();
	void SetState(State newState);
	const string GetID();
	void SetID(string ID);
	string GetIpAddr();
	void SetRoom(string roomName);
	void SetRoomInTime(string roomInTime);
	string GetRoomInTime();
};

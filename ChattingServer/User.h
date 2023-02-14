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
	string ipAddr;
	State state;
	string id;
	string roomName;
	int roomNum;
	string roomInTime;//방에 참가한 시간

public:
	User();
	User(SOCKET socketNum, string ipAddr);

	State GetState();
	void SetState(State newState);
	string GetID();
	void SetID(string ID);
	SOCKET GetSocket();
	string GetIpAddr();
	string GetRoom();
	void SetRoom(string roomName);
	void SetRoomNum(int roomNum);
	int GetRoomNum();
	void SetRoomInTime(string roomInTime);
	string GetRoomInTime();
};

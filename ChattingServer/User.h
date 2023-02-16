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
	ROOM,
	LOGOUT
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
	string buffer;

	User();
	User(SOCKET socketNum, const string& ipAddr);

	State GetState();
	void SetState(const State& newState);
	string GetID();
	void SetID(const string& ID);
	SOCKET GetSocket();
	string GetIpAddr();
	string GetRoom();
	void SetRoom(const string& roomName);
	void SetRoomNum(int roomNum);
	int GetRoomNum();
	void SetRoomInTime(const string& roomInTime);
	string GetRoomInTime();
};

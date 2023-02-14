// -----------------------------------------------------------------------------------
//  Ŭ���̾�Ʈ�� �Է¿� ���� ��� ����
// -----------------------------------------------------------------------------------
#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include "Data.h"
#include "Room.h"
#include "User.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>


/*
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
*/

class Manager
{
private:
	Data Data;

public:
	map<SOCKET, User> userAry;//socket�� client ���� �ڷ���
	map<string, User> nameAry;
	vector<Room> roomAry;

	Manager();
	User* GetUserFromSock(SOCKET sockNum);
	void SetUserToUserSockAry(SOCKET sockNum,User user);

	void LogIn(SOCKET sockNum, string id);
	void ShowAllCommand(SOCKET sockNum);
	void ShowUserList(SOCKET sockNum);
	void ShowRoomList(SOCKET sockNum);
	void ShowRoomInfo(SOCKET sockNum);
	void ShowUserInfo(SOCKET sockNum);
	void TO(SOCKET sockNum);
	void MakeRoom(SOCKET sockNum, int maxClnt, string roomName);
	void J(SOCKET sockNum);
	void X(SOCKET sockNum);
	void NotExistingCommend(SOCKET sockNum);
};

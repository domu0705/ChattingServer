// -----------------------------------------------------------------------------------
//  Ŭ���̾�Ʈ�� �Է¿� ���� ��� ����
// -----------------------------------------------------------------------------------
#pragma once
#pragma comment(lib, "Ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime> 
#include <format>
#include "Data.h"
#include "Room.h"
#include "User.h"


class Manager
{
private:
	int roomIdx;

public:
	map<SOCKET, User> userAry;//socket�� client ���� �ڷ���
	map<string, User*> nameAry;
	vector<Room> roomAry;
	Data* Data;

	Manager();
	static Manager& GetInstance() {
		static Manager s;
		return s;
	}

	User* GetUserFromSock(SOCKET sockNum);
	bool CanStoI(const string& str);
	void SetUserToUserSockAry(SOCKET sockNum,User user);
	void LogIn(SOCKET sockNum, const string& id);
	void ShowAllCommand(SOCKET sockNum);
	void ShowUserList(SOCKET sockNum);
	void ShowRoomList(SOCKET sockNum);
	void ShowRoomInfo(SOCKET sockNum, const string& roomIdx);
	void ShowUserInfo(SOCKET sockNum, const string& targetUserID);
	void MakeRoom(SOCKET sockNum, const string& maxClnt, const string& roomName);
	void JoinRoom(SOCKET sockNum, const string& roomNum);
	void ExitSystem(SOCKET sockNum);
	void NotExistingCommend(SOCKET sockNum);
	void DeleteRoom(SOCKET sockNum);
	void ExitRoom(SOCKET sockNum);
	string GetCurTime();
	void SendMsgToRoom(User* user, const string& msg);
	void SendMsgToUser(SOCKET sockNum, const string& toUser, const string& msg);//���� ������
};

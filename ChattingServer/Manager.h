// -----------------------------------------------------------------------------------
//  클라이언트의 입력에 따른 기능 제공
// -----------------------------------------------------------------------------------
#pragma once
#pragma comment(lib, "Ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS

#include "Data.h"
#include "Room.h"
#include "User.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime> 
#include <format>


class Manager
{
private:
	Data Data;
	int roomIdx;

public:
	map<SOCKET, User> userAry;//socket과 client 묶을 자료형
	map<string, User*> nameAry;
	vector<Room> roomAry;

	Manager();
	static Manager& GetInstance() {
		static Manager s;
		return s;
	}

	User* GetUserFromSock(SOCKET sockNum);
	void SetUserToUserSockAry(SOCKET sockNum,User user);
	void LogIn(SOCKET sockNum, string id);
	void ShowAllCommand(SOCKET sockNum);
	void ShowUserList(SOCKET sockNum);
	void ShowRoomList(SOCKET sockNum);
	void ShowRoomInfo(SOCKET sockNum, int roomIdx);
	void ShowUserInfo(SOCKET sockNum, string targetUserID);
	void MakeRoom(SOCKET sockNum, int maxClnt, string roomName);
	void JoinRoom(SOCKET sockNum,int roomNum);
	void ExitSystem(SOCKET sockNum);
	void NotExistingCommend(SOCKET sockNum);
	void DeleteRoom(SOCKET sockNum);
	void ExitRoom(SOCKET sockNum);
	string GetCurTime();
	void SendMsgToRoom(User* user,string msg);
	void SendMsgToUser(SOCKET sockNum, string toUser, string msg);//쪽지 보내기
};

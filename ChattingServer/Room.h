// -----------------------------------------------------------------------------------
//  채팅방
// -----------------------------------------------------------------------------------
#pragma once

#include "User.h"
#include <vector>
#include <set>
#include <string>
#include <format>

using namespace std;

class Room
{

private:
	int roomIdx;
	bool isOpen;
	string name;
	string owner;
	string genTime;//시:분:초
	int curClntNum;
	int maxClntNum;
	set<User*> userAry;

public:
	Room();

	void SetRoom(int roomIdx, string name, string owner, string genTime, int maxClnt);
	void EnterUser(User* user,string enterTime);
	void ExitUser(User* user);
	int GetRoomIdx();
	int GetCurClntNum();
	int GetMaxClntNum();
	void SetMaxClntNum(int maxClntNum);
	set<User*> GetUserAry();
	string GetCurRoomInfo();
	void SendMsgToRoom(string msg);
	void CloseRoom();
	void ExitRoom(User* user);
	bool GetIsOpen();
	string GetRoomName();
};

// -----------------------------------------------------------------------------------
//  Ã¤ÆÃ¹æ
// -----------------------------------------------------------------------------------
#pragma once

#include <vector>
#include <set>
#include <string>
#include <format>
#include "User.h"
#include "Data.h"


using namespace std;

class Manager;

class Room
{

private:
	int roomIdx;
	bool isOpen;
	string name;
	string owner;
	string genTime;
	int curClntNum;
	int maxClntNum;
	set<User*> userAry;
	Data* Data;

public:
	//Room();
	Room(int idx, const string& roomName, const string& id, const string& time, int num);
	//void SetRoom(int roomIdx, const string& name, const string& owner, const string& genTime, int maxClnt);
	void EnterUser(User* user, const string& enterTime);
	void ExitUser(User* user);
	int GetRoomIdx();
	int GetCurClntNum();
	int GetMaxClntNum();
	void SetMaxClntNum(int maxClntNum);
	set<User*> GetUserAry();
	string GetCurRoomInfo();
	void SendMsgToRoom(const string& msg);
	void CloseRoom();
	void ExitRoom(User* user);
	bool GetIsOpen();
	const string& GetRoomName();
};

// -----------------------------------------------------------------------------------
//  채팅방
// -----------------------------------------------------------------------------------
#pragma once

#include "User.h"
#include <vector>
#include <string>

using namespace std;

class Room
{
	int roomIdx;
	string name;
	string owner;
	bool isOpen;
	string genTime;//시:분:초
	int curClntNum;
	int maxClntNum;

private:
	vector<User*> userAry;

public:
	Room();

	void SetRoom(int roomIdx, string name, string owner, string genTime, int maxClnt);
	void EnterUser(User* user,string enterTime);
	int GetCurClntNum();
	int GetMaxClntNum();
	vector<User*> GetUserAry();
	string GetCurRoomInfo();
};

// -----------------------------------------------------------------------------------
//  Ã¤ÆÃ¹æ
// -----------------------------------------------------------------------------------
#pragma once

#include "User.h"
#include <vector>
#include <string>

using namespace std;

class Room
{
	string name;
	string owner;
	bool isOpen;
	string genTime;
	int maxClnt;

private:
	vector<User> userAry;

public:
	Room();

	void SetRoom(User* user, string name, string owner, string genTime, int maxClnt);
	void SetUser(User* user);
};

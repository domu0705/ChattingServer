#include "User.h"


User::User()
{
	state = WAITING;
}

User::User(SOCKET num, const string& ip)
{
	socketNum = num;
	state = WAITING;
	ipAddr = ip;
}

State User::GetState()
{
	return state;
}

void User::SetState(const State& newState)
{
	state = newState;
}

string User::GetID()
{
	return id;
}

void User::SetID(const string& nickName)
{
	id = nickName;
}

SOCKET User::GetSocket()
{
	return socketNum;
}

string User::GetIpAddr()
{
	return ipAddr;
}

string User::GetRoom()
{
	return roomName;
}

void User::SetRoom(const string& name)
{
	roomName = name;
	SetState(ROOM);
}

void User::SetRoomNum(int num)
{
	roomNum = num;
}

int User::GetRoomNum()
{
	return roomNum ;
}

void User::SetRoomInTime(const string& time)
{
	roomInTime = time;
}

string User::GetRoomInTime()
{
	return roomInTime;
}

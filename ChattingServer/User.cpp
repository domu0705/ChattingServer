#include "USER.h"

User::User()
{
	state = WAITING;
}

User::User(SOCKET num, const string& ip)
{
	this->socketNum = num;
	this->state = WAITING;
	this->ipAddr = ip;
}

State User::GetState()
{
	return state;
}

void User::SetState(const State& newState)
{
	this->state = newState;
	//cout << "SetState = " << state << endl;
}

string User::GetID()
{
	return id;
}

void User::SetID(const string& nickName)
{
	this->id = nickName;
}

SOCKET User::GetSocket()
{
	return this->socketNum;
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
	this->roomName = name;
	SetState(ROOM);
}

void User::SetRoomNum(int num)
{
	roomNum = num;
}

int User::GetRoomNum()
{
	return this->roomNum ;
}


void User::SetRoomInTime(const string& time)
{
	this->roomInTime = time;
}

string User::GetRoomInTime()
{
	return roomInTime;
}

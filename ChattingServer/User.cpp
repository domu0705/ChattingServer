#include "USER.h"

User::User()
{
	state = WAITING;
}
User::User(SOCKET num,string ip)
{
	this->socketNum = num;
	this->state = WAITING;
	this->ipAddr = ip;
}

State User::GetState()
{
	return state;
}

void User::SetState(State newState)
{
	this->state = newState;
	//cout << "SetState = " << state << endl;
}

string User::GetID()
{
	return id;
}

void User::SetID(string nickName)
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

void User::SetRoom(string name)
{
	this->roomName = name;
	SetState(ROOM);
}

void User::SetRoomNum(int num)
{
	this->roomNum = num;
}

int User::GetRoomNum()
{
	return this->roomNum ;
}


void User::SetRoomInTime(string time)
{
	this->roomInTime = time;
}

string User::GetRoomInTime()
{
	return roomInTime;
}

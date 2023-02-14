#include "USER.h"

User::User()
{
	state = WAITING;
}
User::User(SOCKET _socketNum,string _ipAddr)
{
	this->socketNum = _socketNum;
	this->state = WAITING;
	this->ipAddr = _ipAddr;
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

void User::SetID(string _id)
{
	this->id = _id;
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

void User::SetRoom(string _roomName)
{
	this->roomName = _roomName;
	SetState(ROOM);
}

void User::SetRoomNum(int _roomNum)
{
	this->roomNum = _roomNum;
}

int User::GetRoomNum()
{
	return this->roomNum ;
}


void User::SetRoomInTime(string _roomInTime)
{
	this->roomInTime = _roomInTime;
}

string User::GetRoomInTime()
{
	return roomInTime;
}

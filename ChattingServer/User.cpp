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

const string User::GetID()
{
	return id;
}

void User::SetID(string _id)
{
	this->id = _id;
}

string User::GetIpAddr()
{
	return ipAddr;
}

void User::SetRoom(string _roomName)
{
	this->roomName = _roomName;
	SetState(ROOM);
}

void User::SetRoomInTime(string _roomInTime)
{
	this->roomInTime = _roomInTime;
}

string User::GetRoomInTime()
{
	return roomInTime;
}

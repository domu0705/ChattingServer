#include "USER.h"

User::User()
{
	state = WAITING;
}
User::User(SOCKET _socketNum)
{
	socketNum = _socketNum;
	state = WAITING;
}

State User::GetState()
{
	return state;
}

void User::SetState(State newState)
{
	state = newState;
	cout << "SetState = " << state << endl;
}

const string User::GetID()
{
	return ID;
}

void User::SetID(string _ID)
{
	ID = _ID;
}


void User::SetRoom(string _roomName)
{
	roomName = _roomName;
	SetState(ROOM);
}

void User::SetRoomInTime(string _roomInTime)
{
	roomInTime = _roomInTime;
}

string User::GetRoomInTime()
{
	return roomInTime;
}

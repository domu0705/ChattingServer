#include "USER.h"


User::User()
{
	state = Waiting;
}
User::User(SOCKET _socketNum)
{
	socketNum = _socketNum;
	state = Waiting;
}

State User::GetState()
{
	return state;
}

void User::SetState(State newState)
{
	state = newState;
}

string User::GetID()
{
	return ID;
}

void User::SetID(string id)
{
	ID = id;
	return;
}



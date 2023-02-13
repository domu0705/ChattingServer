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

string User::GetID()
{
	return ID;
}

void User::SetID(string id)
{
	ID = id;
	return;
}



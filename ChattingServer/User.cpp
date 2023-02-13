// -----------------------------------------------------------------------------------
//  사용자의 상태 관리
// -----------------------------------------------------------------------------------
#include "USER.h"


USER::USER(SOCKET _socketNum)
{
	socketNum = _socketNum;
	isLogIn = false;
}

bool USER::GetIsLogIn()
{
	return isLogIn;
}

string USER::GetID()
{
	return ID;
}

void USER::SetID(string id)
{
	ID = id;
	return;
}



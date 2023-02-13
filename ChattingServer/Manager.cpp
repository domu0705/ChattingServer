#include "Manager.h"

Manager::Manager()
{

}


User Manager::GetUserFromSock(SOCKET sockNum)
{
	User user = userAry[sockNum];
	return user;
}

void Manager::SetUserToUserSockAry(SOCKET sockNum, User user)
{
	userAry.insert(make_pair(sockNum, user));
}
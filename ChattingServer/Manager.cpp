#include "Manager.h"

Manager::Manager()
{

}


User Manager::GetUserFromSock(SOCKET sockNum)
{
	User user = userSockAry[sockNum];
	return user;
}

void Manager::SetUserToUserSockAry(SOCKET sockNum, User user)
{
	userSockAry.insert(make_pair(sockNum, user));
}
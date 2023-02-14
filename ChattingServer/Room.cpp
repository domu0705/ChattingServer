#include "Room.h"

Room::Room()
{

}

void Room::SetRoom(User* user, string _name, string _owner, string _genTime, int _maxClnt)
{
	name = _name;
	owner = _owner;
	isOpen = true;
	genTime = _genTime;
	maxClnt = _maxClnt;
	SetUser(user);
}

void Room::SetUser(User* user) // user는 진짜 유저의 주소값.
{
	userAry.push_back(*user);
	user->SetRoom(name);
	user->SetState(ROOM);
}
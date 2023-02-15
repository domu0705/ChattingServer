#include "Room.h"

Room::Room()
{
	curClntNum = 0;
}

void Room::SetRoom(int _roomIdx, string _name, string _owner, string _genTime, int _maxClnt)
{
	roomIdx = _roomIdx;
	name = _name;
	owner = _owner;
	isOpen = true;
	genTime = _genTime;
	maxClntNum = _maxClnt;
}

void Room::EnterUser(User* user, string enterTime) // user�� ��¥ ������ �ּҰ�.
{
	userAry.insert(user);
	user->SetRoom(name);
	user->SetRoomNum(roomIdx);
	user->SetState(ROOM);
	user->SetRoomInTime(enterTime);
	++curClntNum;
}

void Room::ExitUser(User* user)// user�� ��¥ ������ �ּҰ�.
{
	user->SetRoom("");
	user->SetRoomNum(-1);
	user->SetState(LOBBY);
	user->SetRoomInTime("");
	--curClntNum;
}

int Room::GetRoomIdx()
{
	return roomIdx;
}

set<User*> Room::GetUserAry()
{
	return userAry;
}

int Room::GetCurClntNum()
{
	return curClntNum;
}

int Room::GetMaxClntNum() 
{
	return maxClntNum;
}

void Room::SetMaxClntNum(int _maxClntNum)
{
	maxClntNum = _maxClntNum;
}

string Room::GetCurRoomInfo()
{
	string header= "------------------------- ��ȭ�� ���� -------------------------\n\r";
	string roomInfo = "[" + to_string(roomIdx+1) + "] (" + to_string(curClntNum) + "/"+ to_string(maxClntNum)+") "+name+"\n\r�����ð�: "+ genTime;
	string peopleInfo = "";

	for (auto iter = userAry.begin();iter!=userAry.end();iter++)
	{
		peopleInfo += "\n\r������: "+ (*iter)->GetID() + "\t\t �����ð�: " + (*iter)->GetRoomInTime();
	}
	string  boundary= "\n\r----------------------------------------------------------------\n\r���ɾ�ȳ�(H) ����(X)\n\r����>";

	return header + roomInfo + peopleInfo + boundary;
}

void Room::SendMsgToRoom(string msg)
{
	//����� ��� Ŭ���̾�Ʈ���� �޼��� ������
	for (auto iter = userAry.begin();iter != userAry.end();iter++)
	{
		send((*iter)->GetSocket(), msg.c_str(), int(msg.size()), 0);
	}
}

void Room::CloseRoom()
{
	isOpen = false;
}

void Room::ExitRoom(User* user)
{
	//�� �����ٴ� �޼��� ����
	string msgInfo = format("{} ���� ���� �����̽��ϴ�.\n\r", user->GetID());
	SendMsgToRoom(msgInfo);
	//�� ���� ���ο��Դ� ���� �ٽ� ����
	string msg = "���ɾ�ȳ�(H) ����(X)\n\r";
	send(user->GetSocket(), msg.c_str(), int(msg.size()), 0);

	//������ ������
	user->SetState(State::LOBBY);
	userAry.erase(user);
	--curClntNum;
}

bool Room::GetIsOpen()
{
	return isOpen;
}

string Room::GetRoomName()
{
	return name;
}
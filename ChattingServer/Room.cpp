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
	cout << "���� curClntNum=" << curClntNum << endl;
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
		//cout << "���� userAry[i].GetRoomInTime() ��� =" << userAry[i]->GetRoomInTime() << endl;
		peopleInfo += "\n\r������: "+ (*iter)->GetID() + "\t\t �����ð�: " + (*iter)->GetRoomInTime();
	}
	string  boundary= "\n\r----------------------------------------------------------------\n\r��ɾ�ȳ�(H) ����(X)\n\r����>";

	return header + roomInfo + peopleInfo + boundary;
}

void Room::SendMsgToRoom(string msg)
{
	//����� ��� Ŭ���̾�Ʈ���� �޼��� ������
	/*
	for (int i = 0;i < curClntNum; ++i)
	{
		send(userAry[i]->GetSocket(), msg.c_str(), int(msg.size()), 0);
	}*/
	for (auto iter = userAry.begin();iter != userAry.end();iter++)
	{
		send((*iter)->GetSocket(), msg.c_str(), int(msg.size()), 0);
	}
}

void Room::CloseRoom()
{
	isOpen = false;
}

bool Room::GetIsOpen()
{
	return isOpen;
}
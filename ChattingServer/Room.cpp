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
	userAry.push_back(user);
	user->SetRoom(name);
	user->SetState(ROOM);
	user->SetRoomInTime(enterTime);
	++curClntNum;
	cout << "���� curClntNum=" << curClntNum << endl;
}

vector<User*> Room::GetUserAry()
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

string Room::GetCurRoomInfo()
{
	string header= "------------------------- ��ȭ�� ���� -------------------------\n\r";
	string roomInfo = "[" + to_string(roomIdx) + "] (" + to_string(curClntNum) + "/"+ to_string(maxClntNum)+") "+name+"\n\r�����ð�: "+ genTime;
	string peopleInfo = "";

	for (int i = 0;i < curClntNum;++i)
	{
		if (!userAry[i]) continue;//nullptrȮ��

		cout << "���� userAry[i].GetRoomInTime() ��� =" << userAry[i]->GetRoomInTime() << endl;
		peopleInfo += "\n\r������: "+userAry[i]->GetID() + "\t\t �����ð�: " + userAry[i]->GetRoomInTime();
	}
	string  boundary= "\n\r----------------------------------------------------------------\n\r��ɾ�ȳ�(H) ����(X)\n\r����>";

	return header + roomInfo + peopleInfo + boundary;
}

void Room::SendMsgToRoom(string msg)
{
	//����� ��� Ŭ���̾�Ʈ���� �޼��� ������
	for (int i = 0;i < curClntNum; ++i)
	{
		send(userAry[i]->GetSocket(), msg.c_str(), int(msg.size()), 0);
	}
}
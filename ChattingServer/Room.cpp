#include "Manager.h"
#include "Room.h"


Room::Room(int idx, const string& roomName, const string& id, const string& time, int num)
{
	roomIdx = idx;
	name = roomName;
	owner = id;
	isOpen = true;
	genTime = time;
	maxClntNum = num;

	curClntNum = 0;
	Data = &Data::GetInstance();
}

void Room::EnterUser(User* user, const string& enterTime) // user�� ��¥ ������ �ּҰ�.
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
	if (!user) return;

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

void Room::SetMaxClntNum(int num)
{
	maxClntNum = num;
}

string Room::GetCurRoomInfo()
{
	const string& header= Data->dataKey[ROOM_LIST_INFO];
	const string& roomInfo = "[" + to_string(roomIdx+1) + "] (" + to_string(curClntNum) + "/"+ to_string(maxClntNum)+") "+name+"\n\r�����ð�: "+ genTime;
	string peopleInfo = "";

	for (auto iter = userAry.begin();iter!=userAry.end();iter++)
	{
		peopleInfo += "\n\r������: "+ (*iter)->GetID() + "\t\t �����ð�: " + (*iter)->GetRoomInTime();
	}
	const string& boundary= Data->dataKey[Sentance_LT_BOUNDARY];
	const string& msg = format("{}{}{}\n\r{}", header,roomInfo,peopleInfo,boundary);
	return msg;
}

void Room::SendMsgToRoom(const string& msg)
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
	const string& msgInfo = format("{} {}", user->GetID(), Data->dataKey[ROOM_EXIT]);
	SendMsgToRoom(msgInfo);

	//�� ���� ���ο��Դ� ���� �ٽ� ����
	const string& msg = Data->dataKey[HELP_LITTLE];
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

const string& Room::GetRoomName()
{
	return name;
}
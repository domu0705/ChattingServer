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

void Room::EnterUser(User* user, string enterTime) // user는 진짜 유저의 주소값.
{
	userAry.insert(user);
	user->SetRoom(name);
	user->SetRoomNum(roomIdx);
	user->SetState(ROOM);
	user->SetRoomInTime(enterTime);
	++curClntNum;
}

void Room::ExitUser(User* user)// user는 진짜 유저의 주소값.
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
	string header= "------------------------- 대화방 정보 -------------------------\n\r";
	string roomInfo = "[" + to_string(roomIdx+1) + "] (" + to_string(curClntNum) + "/"+ to_string(maxClntNum)+") "+name+"\n\r개설시간: "+ genTime;
	string peopleInfo = "";

	for (auto iter = userAry.begin();iter!=userAry.end();iter++)
	{
		peopleInfo += "\n\r참여자: "+ (*iter)->GetID() + "\t\t 참여시간: " + (*iter)->GetRoomInTime();
	}
	string  boundary= "\n\r----------------------------------------------------------------\n\r명령어안내(H) 종료(X)\n\r선택>";

	return header + roomInfo + peopleInfo + boundary;
}

void Room::SendMsgToRoom(string msg)
{
	//방안의 모든 클라이언트에게 메세지 보내기
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
	//방 나갔다는 메세지 전송
	string msgInfo = format("{} 님이 방을 나가셨습니다.\n\r", user->GetID());
	SendMsgToRoom(msgInfo);
	//방 나간 개인에게는 도움말 다시 전달
	string msg = "명령어안내(H) 종료(X)\n\r";
	send(user->GetSocket(), msg.c_str(), int(msg.size()), 0);

	//실제로 나가기
	user->SetState(State::LOBBY);
	userAry.erase(user);
	--curClntNum;
}

bool Room::GetIsOpen()
{
	return isOpen;
}
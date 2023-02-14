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
	userAry.push_back(user);
	user->SetRoom(name);
	user->SetState(ROOM);
	user->SetRoomInTime(enterTime);
	++curClntNum;
	cout << "현재 curClntNum=" << curClntNum << endl;
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
	string header= "------------------------- 대화방 정보 -------------------------\n\r";
	string roomInfo = "[" + to_string(roomIdx) + "] (" + to_string(curClntNum) + "/"+ to_string(maxClntNum)+") "+name+"\n\r개설시간: "+ genTime;
	string peopleInfo = "";

	for (int i = 0;i < curClntNum;++i)
	{
		if (!userAry[i]) continue;//nullptr확인

		cout << "유저 userAry[i].GetRoomInTime() 결과 =" << userAry[i]->GetRoomInTime() << endl;
		peopleInfo += "\n\r참여자: "+userAry[i]->GetID() + "\t\t 참여시간: " + userAry[i]->GetRoomInTime();
	}
	string  boundary= "\n\r----------------------------------------------------------------\n\r명령어안내(H) 종료(X)\n\r선택>";

	return header + roomInfo + peopleInfo + boundary;
}

void Room::SendMsgToRoom(string msg)
{
	//방안의 모든 클라이언트에게 메세지 보내기
	for (int i = 0;i < curClntNum; ++i)
	{
		send(userAry[i]->GetSocket(), msg.c_str(), int(msg.size()), 0);
	}
}
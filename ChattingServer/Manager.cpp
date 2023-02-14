#include "Manager.h"

Manager::Manager()
{
	roomIdx = 0;
	Data.GenerateData();
}


User* Manager::GetUserFromSock(SOCKET sockNum)
{
	User *user = &userAry[sockNum];
	return user;
}

void Manager::SetUserToUserSockAry(SOCKET sockNum, User user)
{
	userAry.insert(make_pair(sockNum, user));

}

void Manager::LogIn(SOCKET sockNum, string id)//명령어 안내
{
	string msg = "\r\n----------------------------------------------\n\r 반갑습니다. 텍스트 채팅 서버 ver 0.1 입니다.\n\r 이용중 불편하신 점이 있으면 아래 이메일로 문의 바랍니다.\n\r 감사합니다.\n\r programmed & arranged by Minjee Kim\n\r email: minjee.kim@nm-neo.com\n\r----------------------------------------------\n\r명령어안내(H) 종료(X)\n\r";
	send(sockNum, msg.c_str(), int(msg.size()), 0);
	cout << "User ID : " << id << endl;
	userAry[sockNum].SetID(id);
	userAry[sockNum].SetState(State::LOBBY);
	nameAry.insert(make_pair(id, userAry[sockNum]));
}

void Manager::ShowAllCommand(SOCKET sockNum)//H: 명령어 안내
{
	string msg = "---------------------------------------------------------------\n\rH                         명령어 안내\n\rUS                        이용자 목록 보기\n\rLT                        대화방 목록 보기\n\rST [방번호]               대화방 정보 보기\n\rPF [상대방ID]             이용자 정보 보기\n\rDEL                         방 폭파\n\rQ                         방 나가기\n\rX                         끝내기\n\r---------------------------------------------------------------\n\r";
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowUserList(SOCKET sockNum)//US : 이용자 목록 보기 ()
{
	string header = "------------------------- 이용자 목록 -------------------------\n\r";
	string userInfo = "";
	for (auto iter = nameAry.begin(); iter != nameAry.end(); iter++)
	{
		userInfo += "이용자: " + iter->first + "\t접속지: " + iter->second.GetIpAddr() + "\n\r";
		//cout << userID << "\n";
	}
	string msg = format("{}{}{}", header, userInfo,"\n\r명령어안내(H) 종료(X)\n\r");
	send(sockNum,  +msg.c_str(), int(msg.size()), 0);
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::ShowRoomList(SOCKET sockNum)//LT : 대화방 목록 보기
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::ShowRoomInfo(SOCKET sockNum,int _roomIdx)//ST :L 대화방 정보 보기
{
	if (_roomIdx > roomAry.size()|| _roomIdx <1 || !roomAry[_roomIdx - 1].GetIsOpen())//방이 없거나 이미 닫혀있다면
	{
		string msg = "** 해당 번호의 방은 존재하지 않습니다.\n\r";
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
	else
	{
		string msg = roomAry[_roomIdx-1].GetCurRoomInfo();
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
}

void Manager::ShowUserInfo(SOCKET sockNum)//PF: 이용자 정보 보기
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::TO(SOCKET sockNum)//쪽지 보내기
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::MakeRoom(SOCKET sockNum, int maxClnt, string roomName)//O: 대화방 만들기
{
	cout << "MakeRoom - sockNum ID : " << sockNum << endl;
	User* user = GetUserFromSock(sockNum);

	//새로운 방 생성
	Room newRoom;
	newRoom.SetRoom(roomIdx, roomName, user->GetID(), GetCurTime(), maxClnt);
	roomAry.push_back(newRoom);
	roomAry[roomIdx].EnterUser(user, GetCurTime());

	//클라 알림보내기
	string str = "** 대화방이 개설되었습니다.\n\r";

	send(sockNum, str.c_str(), int(str.size()), 0);
	string msg = format("** {}님이 들어오셨습니다. (현재인원 {}/{})\n\r", user->GetID(), roomAry[roomIdx].GetCurClntNum(), roomAry[roomIdx].GetMaxClntNum());
	roomAry[roomIdx].SendMsgToRoom(msg);
	++roomIdx;
}

void Manager::JoinRoom(SOCKET sockNum, int roomNum)//대화방 참여하기
{
	if (roomNum > roomAry.size() || roomNum < 1)
	{
		string msg = "** 해당 방은 존재하지 않습니다. \n\r명령어안내(H) 종료(X)\n\r";
		send(sockNum, msg.c_str(), int(msg.size()), 0);
		
	}
	else //방 번호는 존재
	{
		if (roomAry[roomNum - 1].GetMaxClntNum() <= roomAry[roomNum - 1].GetCurClntNum())
		{
			string msg = "** 인원이 꽉 차서 참여할 수 없습니다.\n\r명령어안내(H) 종료(X)\n\r";
			send(sockNum, msg.c_str(), int(msg.size()), 0);
		}
		else
		{
			User* user = GetUserFromSock(sockNum);
			roomAry[roomNum-1].EnterUser(user, GetCurTime());

			string msg = format("** {}님이 들어오셨습니다. (현재인원 {}/{})\n\r", user->GetID(), roomAry[roomNum - 1].GetCurClntNum(), roomAry[roomNum - 1].GetMaxClntNum());
			roomAry[roomNum - 1].SendMsgToRoom(msg);
		}
	}
	
}

void Manager::X(SOCKET sockNum)//끝내기
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::NotExistingCommend(SOCKET sockNum)//끝내기
{
	string msg = "\n\r존재하지 않는 명령입니다. \n\r명령어안내(H) 종료(X)\n\r";
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::DeleteRoom(SOCKET sockNum)//확인
{
	User* user = GetUserFromSock(sockNum);
	int curRoomNum = user->GetRoomNum();
	//int curClntNum = roomAry[curRoomNum]->GetCurClntNum();
	set<User*> roomUserAry = roomAry[curRoomNum].GetUserAry();

	roomAry[curRoomNum].SetMaxClntNum(0);//방 폭파
	roomAry[curRoomNum].CloseRoom();
	//curClent 도 0돼야 함 수정 userAr

	string msg = "\n\r해당 방은 폭파되었습니다. 로비로 이동합니다.\n\r명령어안내(H) 종료(X)\n\r";

	//방 안의 사람들 내보내기
	for (auto iter = roomUserAry.begin();iter != roomUserAry.end();iter++)
	{
		(*iter)->SetState(LOBBY);
		send((*iter)->GetSocket(), msg.c_str(), int(msg.size()), 0);
	}

}

void Manager::ExitRoom(SOCKET sockNum)
{
	User* user = GetUserFromSock(sockNum);
	roomAry[user->GetRoomNum()].ExitRoom(user);
}

string Manager::GetCurTime() 
{
	string curTime = "";
	time_t timer = time(NULL);
	struct tm* t = localtime(&timer);

	string hour = to_string(t->tm_hour);
	string min = to_string(t->tm_min);
	string sec = to_string(t->tm_sec);

	hour = hour.length() > 1 ? hour: "0" + hour;
	min = min.length() > 1 ? min: "0" + min;
	sec = sec.length() > 1 ? sec: "0" + sec;
	curTime = hour + ":" + min + ":" + sec;

	return curTime;
}

void Manager::SendMsgToRoom(User* user, string msg) // room에서 채팅 보내기
{
	string msgInfo = format("{} > {}\n\r", user->GetID(), msg);
	roomAry[user->GetRoomNum()].SendMsgToRoom(msgInfo);
}

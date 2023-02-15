#include "Manager.h"

Manager::Manager()
{
	roomIdx = 0;
	Data = &Data::GetInstance();
	//Data.GenerateData();
	cout << "Data generated";
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

void Manager::LogIn(SOCKET sockNum, const string& id)//명령어 안내
{
	auto iter = nameAry.find(id);
	if (iter != nameAry.end())//중복 아이디 존재
	{
		string msg = Data->dataKey[LOGIN_ID_DUP];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
	else {
		string msg = Data->dataKey[LOGIN_HI];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
		userAry[sockNum].SetID(id);
		userAry[sockNum].SetState(State::LOBBY);
		nameAry.insert(make_pair(id, &userAry[sockNum]));
	}
}

void Manager::ShowAllCommand(SOCKET sockNum)//H: 명령어 안내
{
	string msg = Data->dataKey[COMMAND_HELP];
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowUserList(SOCKET sockNum)//US : 이용자 목록 보기 ()
{
	string userInfo = "";
	for (auto iter = nameAry.begin(); iter != nameAry.end(); iter++)
	{
		userInfo += "이용자: " + iter->first + "\t접속지: " + iter->second->GetIpAddr() + "\n\r";
	}
	string msg = format("{}{}{}", Data->dataKey[USERLIST_HEADER], userInfo, Data->dataKey[HELP_LITTLE]);
	send(sockNum,  +msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowRoomList(SOCKET sockNum)//LT : 대화방 목록 보기
{
	string header = Data->dataKey[Sentance_LT_HEADER];
	string roomInfo = "";
	for (int i = 0;i< roomAry.size();i++)
	{
		if (roomAry[i].GetIsOpen())
		{
			roomInfo += "[" + to_string(roomAry[i].GetRoomIdx()+1) + "] (" + to_string(roomAry[i].GetCurClntNum()) + "/" + to_string(roomAry[i].GetMaxClntNum()) + ") " + roomAry[i].GetRoomName()+"\n\r";
		}
	}
	string  boundary = Data->dataKey[Sentance_LT_BOUNDARY];;

	string msg = format("{}{}{}", header, roomInfo, boundary);
	send(sockNum, +msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowRoomInfo(SOCKET sockNum,int roomIndex)//ST :L 대화방 정보 보기
{
	if (roomIndex > roomAry.size()|| roomIndex <1 || !roomAry[roomIndex - 1].GetIsOpen())//방이 없거나 이미 닫혀있다면
	{
		string msg = Data->dataKey[ROOM_NUM_NOT_EXIST];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
	else
	{
		string msg = roomAry[roomIndex-1].GetCurRoomInfo();
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
}

void Manager::ShowUserInfo(SOCKET sockNum, const string& targetUserID)//PF: 이용자 정보 보기
{
	auto iter = nameAry.find(targetUserID);
	if (iter != nameAry.end()) 
	{
		if (iter->second->GetState() == 2)
		{
			string msg = format("** {}{}{}{}", targetUserID, Data->dataKey[ROOM_SENT_1],iter->second->GetRoom(), Data->dataKey[ROOM_SENT_2]);
			send(sockNum, msg.c_str(), int(msg.size()), 0);
		}
		else
		{
			string msg = format("** {}{}", targetUserID, Data->dataKey[IN_LOBBY]);
			send(sockNum, msg.c_str(), int(msg.size()), 0);
		}
	}
	else {
		string msg = format("** {}{}", targetUserID, Data->dataKey[USER_INFO_NON]);
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
}

void Manager::MakeRoom(SOCKET sockNum, int maxClnt, const string& roomName)//O: 대화방 만들기
{
	if (maxClnt > 20 || maxClnt < 2)
	{
		string msg = Data->dataKey[ROOM_MAX_CLNT];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
	else
	{
		User* user = GetUserFromSock(sockNum);

		//새로운 방 생성
		Room newRoom;
		newRoom.SetRoom(roomIdx, roomName, user->GetID(), GetCurTime(), maxClnt);
		roomAry.push_back(newRoom);
		roomAry[roomIdx].EnterUser(user, GetCurTime());

		//클라 알림보내기
		string str = Data->dataKey[ROOM_GEN];

		send(sockNum, str.c_str(), int(str.size()), 0);
		string msg = format("** {}{} {}/{})\n\r", user->GetID(), Data->dataKey[OTHER_ENTERED],roomAry[roomIdx].GetCurClntNum(), roomAry[roomIdx].GetMaxClntNum());
		roomAry[roomIdx].SendMsgToRoom(msg);
		++roomIdx;
	}
}

void Manager::JoinRoom(SOCKET sockNum, int roomNum)//대화방 참여하기
{
	if (roomNum > roomAry.size() || roomNum < 1)
	{	
		string msg = format("{}{}", Data->dataKey[ROOM_NOT_EXIST], Data->dataKey[HELP_LITTLE]);
		send(sockNum, msg.c_str(), int(msg.size()), 0);
		
	}
	else //방 번호는 존재
	{
		if (roomAry[roomNum - 1].GetMaxClntNum() <= roomAry[roomNum - 1].GetCurClntNum())
		{
			string msg = format("{}{}", Data->dataKey[ROOM_MAX], Data->dataKey[HELP_LITTLE]);
			send(sockNum, msg.c_str(), int(msg.size()), 0);
		}
		else
		{
			User* user = GetUserFromSock(sockNum);
			roomAry[roomNum-1].EnterUser(user, GetCurTime());

			//타 유저가 입장했다는 메세지 보내기
			string msg = format("** {}{} {}/{})\n\r", user->GetID(), Data->dataKey[OTHER_ENTERED], roomAry[roomNum - 1].GetCurClntNum(), roomAry[roomNum - 1].GetMaxClntNum());
			roomAry[roomNum - 1].SendMsgToRoom(msg);
		}
	}
	
}

void Manager::ExitSystem(SOCKET sockNum)//끝내기
{
	User* user = GetUserFromSock(sockNum);
	user->SetState(LOGOUT);
	string msg = Data->dataKey[THX];
	send(sockNum, msg.c_str(), int(msg.size()), 0);
	nameAry.erase(user->GetID());

}

void Manager::NotExistingCommend(SOCKET sockNum)//끝내기
{
	string msg = Data->dataKey[COMM_NOT_EXIST];
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

	string msg = Data->dataKey[ROOM_DEL];

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

void Manager::SendMsgToRoom(User* user, const string& msg) // room에서 방 내부로 채팅 보내기
{
	string msgInfo = format("{} > {}\n\r", user->GetID(), msg);
	roomAry[user->GetRoomNum()].SendMsgToRoom(msgInfo);
}

void Manager::SendMsgToUser(SOCKET fromSockNum, const string& toUser, const string& msg) //쪽지 보내기
{
	auto destUser = nameAry.find(toUser);
	if (destUser != nameAry.end()) {
		User* fromUser = GetUserFromSock(fromSockNum);

		//자기 자신에게 보내는지 확인
		if (fromUser->GetID().compare(toUser) == 0)
		{
			string fromUserMsg = Data->dataKey[MYSELF_NO];
			send(fromSockNum, fromUserMsg.c_str(), int(fromUserMsg.size()), 0);
		}
		else//보내는 유저에게 메세지 전송
		{
			string fromUserMsg = Data->dataKey[SECR_SEND];
			send(fromSockNum, fromUserMsg.c_str(), int(fromUserMsg.size()), 0);

			//받는 유저에게 메세지 전송
			string toUserMsg = format("\n\r# {}{}{}\n\r", fromUser->GetID(), Data->dataKey[SECR], msg);
			send(destUser->second->GetSocket(), toUserMsg.c_str(), int(toUserMsg.size()), 0);
		}
	}
	else {
		string warnMsg = Data->dataKey[USER_NOT_EXIST];
		send(fromSockNum, warnMsg.c_str(), int(warnMsg.size()), 0);
	}
}

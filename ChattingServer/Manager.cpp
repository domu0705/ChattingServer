#include "Manager.h"


Manager::Manager()
{
	roomIdx = 1;
	Data = &Data::GetInstance();
}

bool Manager::CanStoI(const string& str)
{
	bool canStoI = true;
	for (int i = 0;i < str.size();++i)
	{
		if ((int)str[i] < '0' || (int)str[i] > '9')
			canStoI = false;
	}
	return canStoI;
}

User* Manager::GetUserFromSock(SOCKET sockNum)
{
	User* user = userAry[sockNum];
	return user;
}

void Manager::SetUserToUserSockAry(SOCKET sockNum, User* user)
{
	cout << "user=" << user << endl;
	userAry.insert(make_pair(sockNum, user));

}

void Manager::LogIn(SOCKET sockNum, const string& id)//명령어 안내
{
	auto iter = nameAry.find(id);
	if (iter != nameAry.end())//중복 아이디 존재
	{
		cout << "LogIn | 중복 아이디 존재 " << endl;
		const string& msg = Data->dataKey[LOGIN_ID_DUP];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
	else {
		const string& msg = Data->dataKey[LOGIN_HI];
		send(sockNum, msg.c_str(), int(msg.size()), 0);

		if (!userAry[sockNum]) return;
		userAry[sockNum]->SetID(id);
		userAry[sockNum]->SetState(State::LOBBY);
		nameAry.insert(make_pair(id, userAry[sockNum]));
		cout << "[" << id << "]" << "sockNum=" << sockNum << " LOGIN Success " << endl;
	}
}

void Manager::ShowAllCommand(SOCKET sockNum)//H: 명령어 안내
{
	const string& msg = Data->dataKey[COMMAND_HELP];
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowUserList(SOCKET sockNum)//US : 이용자 목록 보기 ()
{
	string userInfo = "";
	for (auto iter = nameAry.begin(); iter != nameAry.end(); iter++)
	{
		userInfo += "이용자: " + iter->first + "\t접속지: " + iter->second->GetIpAddr() + "\r\n";
	}
	const string& msg = format("{}{}{}", Data->dataKey[USERLIST_HEADER], userInfo, Data->dataKey[HELP_LITTLE]);
	send(sockNum, +msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowRoomList(SOCKET sockNum)//LT : 대화방 목록 보기
{
	const string& header = Data->dataKey[Sentance_LT_HEADER];
	string roomInfo = "";
	for (auto iter = roomAry.begin(); iter != roomAry.end(); iter++)
	{
		Room* room = iter->second;
		if (room->GetIsOpen())
		{
			roomInfo += "[" + to_string(room->GetRoomIdx()) + "] (" + to_string(room->GetCurClntNum()) + "/" + to_string(room->GetMaxClntNum()) + ") " + room->GetRoomName() + "\n\r";
		}
	}
	const string& boundary = Data->dataKey[Sentance_LT_BOUNDARY];;
	const string& msg = format("{}{}{}", header, roomInfo, boundary);
	send(sockNum, +msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowRoomInfo(SOCKET sockNum, const string& idx)//ST :L 대화방 정보 보기
{
	int roomIndex = 0;
	if (!CanStoI(idx))
	{
		const string& msg = Data->dataKey[ROOM_NUM_NOT_EXIST];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
		return;
	}

	roomIndex = stoi(idx);

	if (roomAry.find(roomIndex)==roomAry.end() || roomAry[roomIndex]->GetIsOpen() == false)//방이 없거나 이미 닫혀있다면
	{
		const string& msg = Data->dataKey[ROOM_NUM_NOT_EXIST];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
	else
	{
		const string& msg = roomAry[roomIndex]->GetCurRoomInfo();
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
			const string& msg = format("** {}{}{}{}", targetUserID, Data->dataKey[ROOM_SENT_1], iter->second->GetRoom(), Data->dataKey[ROOM_SENT_2]);
			send(sockNum, msg.c_str(), int(msg.size()), 0);
		}
		else
		{
			const string& msg = format("** {}{}", targetUserID, Data->dataKey[IN_LOBBY]);
			send(sockNum, msg.c_str(), int(msg.size()), 0);
		}
	}
	else {
		const string& msg = format("** {}{}", targetUserID, Data->dataKey[USER_INFO_NON]);
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
}

void Manager::MakeRoom(SOCKET sockNum, const string& maxClnt, const string& roomName)//O: 대화방 만들기
{
	int maxClntNum = 0;
	if (!CanStoI(maxClnt))
	{
		const string& msg = Data->dataKey[ROOM_MAX_CLNT];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
		return;
	}

	maxClntNum = stoi(maxClnt);

	if (maxClntNum > 20 || maxClntNum < 2) // 방 개수 이상함
	{
		const string& msg = Data->dataKey[ROOM_MAX_CLNT];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
	else
	{
		User* user = GetUserFromSock(sockNum);

		//새로운 방 생성
		Room* room = new Room(roomIdx, roomName, user->GetID(), GetCurTime(), maxClntNum);
		roomAry.emplace(roomIdx, room);
		roomAry[roomIdx]->EnterUser(user, GetCurTime());

		//클라 알림보내기
		const string& str = Data->dataKey[ROOM_GEN];

		send(sockNum, str.c_str(), int(str.size()), 0);
		const string& msg = format("** {}{} {}/{} \r\n", user->GetID(), Data->dataKey[OTHER_ENTERED], roomAry[roomIdx]->GetCurClntNum(), roomAry[roomIdx]->GetMaxClntNum());
		roomAry[roomIdx]->SendMsgToRoom(msg);
		++roomIdx;
	}
}

void Manager::JoinRoom(SOCKET sockNum, const string& roomNum)//대화방 참여하기
{
	int roomNumber = 0;
	if (!CanStoI(roomNum))
	{
		const string& msg = format("{}{}", Data->dataKey[ROOM_NOT_EXIST], Data->dataKey[HELP_LITTLE]);
		send(sockNum, msg.c_str(), int(msg.size()), 0);
		return;
	}

	roomNumber = stoi(roomNum);

	map<int, Room*>::iterator iter = roomAry.find(roomNumber);
	if (iter == roomAry.end()) {//해당 방이 없다면
		const string& msg = format("{}{}", Data->dataKey[ROOM_NOT_EXIST], Data->dataKey[HELP_LITTLE]);
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
	else //방 번호는 존재
	{
		Room* destRoom = iter->second;
		if (destRoom->GetMaxClntNum() <= destRoom->GetCurClntNum())
		{
			const string& msg = format("{}{}", Data->dataKey[ROOM_MAX], Data->dataKey[HELP_LITTLE]);
			send(sockNum, msg.c_str(), int(msg.size()), 0);
		}
		else
		{
			User* user = GetUserFromSock(sockNum);
			destRoom->EnterUser(user, GetCurTime());

			//타 유저가 입장했다는 메세지 보내기
			const string& msg = format("** {}{} {}/{}) \r\n", user->GetID(), Data->dataKey[OTHER_ENTERED], destRoom->GetCurClntNum(), destRoom->GetMaxClntNum());
			destRoom->SendMsgToRoom(msg);
		}
	}
}

void Manager::DisconnectUser(SOCKET sockNum)//끝내기
{
	User* user = GetUserFromSock(sockNum);
	user->SetState(LOGOUT);
	const string& msg = Data->dataKey[THX];
	send(sockNum, msg.c_str(), int(msg.size()), 0);
	nameAry.erase(user->GetID());
	delete user;

}

void Manager::NotExistingCommend(SOCKET sockNum)//끝내기
{
	const string& msg = Data->dataKey[COMM_NOT_EXIST];
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::DeleteRoom(SOCKET sockNum)//확인
{
	User* user = GetUserFromSock(sockNum);
	int curRoomNum = user->GetRoomNum();
	set<User*> roomUserAry = roomAry[curRoomNum]->GetUserAry();

	roomAry[curRoomNum]->SetMaxClntNum(0);//방 폭파
	roomAry[curRoomNum]->CloseRoom();

	const string& msg = Data->dataKey[ROOM_DEL];

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
	roomAry[user->GetRoomNum()]->ExitRoom(user);
}

string Manager::GetCurTime()
{
	time_t timer = time(NULL);
	struct tm* t = localtime(&timer);

	string hour = to_string(t->tm_hour);
	string min = to_string(t->tm_min);
	string sec = to_string(t->tm_sec);

	hour = hour.length() > 1 ? hour : "0" + hour;
	min = min.length() > 1 ? min : "0" + min;
	sec = sec.length() > 1 ? sec : "0" + sec;

	const string& curTime = format("{}:{}:{}", hour, min, sec);

	return curTime;
}

void Manager::SendMsgToRoom(User* user, const string& msg) // room에서 방 내부로 채팅 보내기
{
	const string& msgInfo = format("{} > {}\r\n", user->GetID(), msg);
	roomAry[user->GetRoomNum()]->SendMsgToRoom(msgInfo);
}

void Manager::SendMsgToUser(SOCKET fromSockNum, const string& toUser, const string& msg) //쪽지 보내기
{
	auto destUser = nameAry.find(toUser);
	if (destUser != nameAry.end()) {
		User* fromUser = GetUserFromSock(fromSockNum);

		if (fromUser->GetID().compare(toUser) == 0)//자기 자신에게 보내는지 확인
		{
			const string& fromUserMsg = Data->dataKey[MYSELF_NO];
			send(fromSockNum, fromUserMsg.c_str(), int(fromUserMsg.size()), 0);
		}
		else//보내는 유저에게 메세지 전송
		{
			cout << "fromSockNum=" << fromSockNum << "  to=" << destUser->second->GetSocket() << endl;
			const string& fromUserMsg = Data->dataKey[SECR_SEND];
			send(fromSockNum, fromUserMsg.c_str(), int(fromUserMsg.size()), 0);

			//받는 유저에게 메세지 전송
			const string& toUserMsg = format("\n\r# {}{}{}\r\n", fromUser->GetID(), Data->dataKey[SECR], msg);
			send(destUser->second->GetSocket(), toUserMsg.c_str(), int(toUserMsg.size()), 0);
		}
	}
	else {
		const string& warnMsg = Data->dataKey[USER_NOT_EXIST];
		send(fromSockNum, warnMsg.c_str(), int(warnMsg.size()), 0);
	}
}

void Manager::HandleState(int state, SOCKET* targetSocket, vector<string>& words, const string& msgBuffer)
{
	if (state == State::WAITING) // LOGIN 이전의 상태.
	{
		HandleWaiting(targetSocket, words);
	}
	else if (state == State::LOBBY)
	{
		HandleLobby(targetSocket, words);
	}
	else if (state == State::ROOM)
	{
		HandleRoom(targetSocket, words, msgBuffer);
	}
}

//클라가 Wait 상태일때 가능한 명령을 실행하는 함수
void Manager::HandleWaiting(SOCKET* targetSocket, vector<string>& words)
{
	if (words[0] == "LOGIN" && words.size() == 2 && words[1].length() > 0)
	{
		LogIn(*targetSocket, words[1]);
		cout << "로그인 시도 *targetSocket=" << *targetSocket << " id= " << words[1];
	}
	else //로그인 실패
	{
		cout << "words[0]=" << words[0] ;
		string msg = "** 올바른 사용법은 LOGIN [ID] 입니다.\r\n";
		send(*targetSocket, msg.c_str(), int(msg.size()), 0);
	}
}

//클라가 Lobby 상태일때 가능한 명령을 실행하는 함수
void Manager::HandleLobby(SOCKET* targetSocket, vector<string>& words)
{
	cout << "words[0]=" << words[0]<<endl;
	if (words.size() > 1)
	{
		cout << "words[1]" << words[1] << endl;
	}
	if (words[0].compare("H") == 0)
	{
		ShowAllCommand(*targetSocket);
	}
	else if (words[0].compare("US") == 0)
	{
		ShowUserList(*targetSocket);
	}
	else if (words[0].compare("LT") == 0)//대화방 목록 보기
	{
		cout << "LT=" << words[0] << endl;
		ShowRoomList(*targetSocket);
	}
	else if (words[0].compare("ST") == 0 && words.size() == 2)
	{
		ShowRoomInfo(*targetSocket, words[1]);
	}
	else if (words[0].compare("PF") == 0 && words.size() == 2)//이용자 정보 보기
	{
		ShowUserInfo(*targetSocket, words[1]);
	}
	else if (words[0].compare("TO") == 0 && words.size() == 3)//쪽지 보내기
	{
		SendMsgToUser(*targetSocket, words[1], words[2]);
	}
	else if (words[0].compare("O") == 0 && words.size() == 3) // 대화방 만들기
	{
		MakeRoom(*targetSocket, words[1], words[2]);
	}
	else if (words[0].compare("J") == 0 && words.size() == 2)
	{
		JoinRoom(*targetSocket, words[1]);
	}
	else if (words[0].compare("X") == 0)
	{
		DisconnectUser(*targetSocket);
	}
	else
	{
		NotExistingCommend(*targetSocket);
	}
}

//클라가 Room 상태일때 가능한 명령을 실행하는 함수
void Manager::HandleRoom(SOCKET* targetSocket, vector<string>& words, const string& msgBuffer)
{
	if (words[0].compare("DEL") == 0)
	{
		DeleteRoom(*targetSocket);
	}
	else if (words[0].compare("Q") == 0)
	{
		ExitRoom(*targetSocket);
	}
	else if (words[0].compare("TO") == 0 && words.size() == 3)//쪽지 보내기
	{
		SendMsgToUser(*targetSocket, words[1], words[2]);
	}
	else
	{
		SendMsgToRoom(GetUserFromSock(*targetSocket), msgBuffer);
	}
}
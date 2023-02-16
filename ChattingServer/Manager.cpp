#include "Manager.h"


Manager::Manager()
{
	roomIdx = 0;
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
	User *user = userAry[sockNum];
	return user;
}

void Manager::SetUserToUserSockAry(SOCKET sockNum, User* user)
{
	cout << "user=" << user << endl;
	userAry.insert(make_pair(sockNum, user));

}

void Manager::LogIn(SOCKET sockNum, const string& id)//��ɾ� �ȳ�
{
	auto iter = nameAry.find(id);
	if (iter != nameAry.end())//�ߺ� ���̵� ����
	{
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
		cout << "[" << id << "]" <<"sockNum="<< sockNum << " LOGIN Success " << endl;
	}
}

void Manager::ShowAllCommand(SOCKET sockNum)//H: ��ɾ� �ȳ�
{
	const string& msg = Data->dataKey[COMMAND_HELP];
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowUserList(SOCKET sockNum)//US : �̿��� ��� ���� ()
{
	string userInfo = "";
	for (auto iter = nameAry.begin(); iter != nameAry.end(); iter++)
	{
		userInfo += "�̿���: " + iter->first + "\t������: " + iter->second->GetIpAddr() + "\n\r";
	}
	const string& msg = format("{}{}{}", Data->dataKey[USERLIST_HEADER], userInfo, Data->dataKey[HELP_LITTLE]);
	send(sockNum,  +msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowRoomList(SOCKET sockNum)//LT : ��ȭ�� ��� ����
{
	const string& header = Data->dataKey[Sentance_LT_HEADER];
	string roomInfo = "";
	for (int i = 0;i< roomAry.size();i++)
	{
		if (roomAry[i].GetIsOpen())
		{
			roomInfo += "[" + to_string(roomAry[i].GetRoomIdx()+1) + "] (" + to_string(roomAry[i].GetCurClntNum()) + "/" + to_string(roomAry[i].GetMaxClntNum()) + ") " + roomAry[i].GetRoomName()+"\n\r";
		}
	}
	const string& boundary = Data->dataKey[Sentance_LT_BOUNDARY];;
	const string& msg = format("{}{}{}", header, roomInfo, boundary);
	send(sockNum, +msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowRoomInfo(SOCKET sockNum, const string& idx)//ST :L ��ȭ�� ���� ����
{
	int roomIndex = 0;
	if (!CanStoI(idx))
	{
		const string& msg = Data->dataKey[ROOM_NUM_NOT_EXIST];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
		return;
	}

	roomIndex = stoi(idx);

	if (roomIndex > roomAry.size()|| roomIndex <1 || !roomAry[roomIndex - 1].GetIsOpen())//���� ���ų� �̹� �����ִٸ�
	{
		const string& msg = Data->dataKey[ROOM_NUM_NOT_EXIST];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
	else
	{
		const string& msg = roomAry[roomIndex -1].GetCurRoomInfo();
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
}

void Manager::ShowUserInfo(SOCKET sockNum, const string& targetUserID)//PF: �̿��� ���� ����
{
	auto iter = nameAry.find(targetUserID);
	if (iter != nameAry.end()) 
	{
		if (iter->second->GetState() == 2)
		{
			const string& msg = format("** {}{}{}{}", targetUserID, Data->dataKey[ROOM_SENT_1],iter->second->GetRoom(), Data->dataKey[ROOM_SENT_2]);
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

void Manager::MakeRoom(SOCKET sockNum, const string& maxClnt, const string& roomName)//O: ��ȭ�� �����
{
	int maxClntNum= 0;
	if (!CanStoI(maxClnt))
	{
		const string& msg = Data->dataKey[ROOM_MAX_CLNT];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
		return;
	}

	maxClntNum = stoi(maxClnt);

	if (maxClntNum > 20 || maxClntNum < 2) // �� ���� �̻���
	{
		const string& msg = Data->dataKey[ROOM_MAX_CLNT];
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
	else
	{
		User* user = GetUserFromSock(sockNum);

		//���ο� �� ����
		roomAry.emplace_back(roomIdx, roomName, user->GetID(), GetCurTime(), maxClntNum);
		roomAry[roomIdx].EnterUser(user, GetCurTime());

		//Ŭ�� �˸�������
		const string& str = Data->dataKey[ROOM_GEN];

		send(sockNum, str.c_str(), int(str.size()), 0);
		const string& msg = format("** {}{} {}/{})\n\r", user->GetID(), Data->dataKey[OTHER_ENTERED],roomAry[roomIdx].GetCurClntNum(), roomAry[roomIdx].GetMaxClntNum());
		roomAry[roomIdx].SendMsgToRoom(msg);
		++roomIdx;
	}
}

void Manager::JoinRoom(SOCKET sockNum, const string& roomNum)//��ȭ�� �����ϱ�
{
	int roomNumber = 0;
	if (!CanStoI(roomNum))
	{
		const string& msg = format("{}{}", Data->dataKey[ROOM_NOT_EXIST], Data->dataKey[HELP_LITTLE]);
		send(sockNum, msg.c_str(), int(msg.size()), 0);
		return;
	}

	roomNumber = stoi(roomNum);

	if (roomNumber > roomAry.size() || roomNumber < 1)
	{	
		const string& msg = format("{}{}", Data->dataKey[ROOM_NOT_EXIST], Data->dataKey[HELP_LITTLE]);
		send(sockNum, msg.c_str(), int(msg.size()), 0);
		
	}
	else //�� ��ȣ�� ����
	{
		if (roomAry[roomNumber - 1].GetMaxClntNum() <= roomAry[roomNumber - 1].GetCurClntNum())
		{
			const string& msg = format("{}{}", Data->dataKey[ROOM_MAX], Data->dataKey[HELP_LITTLE]);
			send(sockNum, msg.c_str(), int(msg.size()), 0);
		}
		else
		{
			User* user = GetUserFromSock(sockNum);
			roomAry[roomNumber -1].EnterUser(user, GetCurTime());

			//Ÿ ������ �����ߴٴ� �޼��� ������
			const string& msg = format("** {}{} {}/{})\n\r", user->GetID(), Data->dataKey[OTHER_ENTERED], roomAry[roomNumber - 1].GetCurClntNum(), roomAry[roomNumber - 1].GetMaxClntNum());
			roomAry[roomNumber - 1].SendMsgToRoom(msg);
		}
	}
}

void Manager::DisconnectUser(SOCKET sockNum)//������
{
	User* user = GetUserFromSock(sockNum);
	user->SetState(LOGOUT);
	const string& msg = Data->dataKey[THX];
	send(sockNum, msg.c_str(), int(msg.size()), 0);
	nameAry.erase(user->GetID());
	delete user;

}

void Manager::NotExistingCommend(SOCKET sockNum)//������
{
	const string& msg = Data->dataKey[COMM_NOT_EXIST];
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::DeleteRoom(SOCKET sockNum)//Ȯ��
{
	User* user = GetUserFromSock(sockNum);
	int curRoomNum = user->GetRoomNum();
	set<User*> roomUserAry = roomAry[curRoomNum].GetUserAry();

	roomAry[curRoomNum].SetMaxClntNum(0);//�� ����
	roomAry[curRoomNum].CloseRoom();

	const string& msg = Data->dataKey[ROOM_DEL];

	//�� ���� ����� ��������
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
	time_t timer = time(NULL);
	struct tm* t = localtime(&timer);

	string hour = to_string(t->tm_hour);
	string min = to_string(t->tm_min);
	string sec = to_string(t->tm_sec);

	hour = hour.length() > 1 ? hour: "0" + hour;
	min = min.length() > 1 ? min: "0" + min;
	sec = sec.length() > 1 ? sec: "0" + sec;

	const string& curTime = format("{}:{}:{}", hour, min, sec);

	return curTime;
}

void Manager::SendMsgToRoom(User* user, const string& msg) // room���� �� ���η� ä�� ������
{
	const string& msgInfo = format("{} > {}\n\r", user->GetID(), msg);
	roomAry[user->GetRoomNum()].SendMsgToRoom(msgInfo);
}

void Manager::SendMsgToUser(SOCKET fromSockNum, const string& toUser, const string& msg) //���� ������
{
	auto destUser = nameAry.find(toUser);
	if (destUser != nameAry.end()) {
		User* fromUser = GetUserFromSock(fromSockNum);

		if (fromUser->GetID().compare(toUser) == 0)//�ڱ� �ڽſ��� �������� Ȯ��
		{
			const string& fromUserMsg = Data->dataKey[MYSELF_NO];
			send(fromSockNum, fromUserMsg.c_str(), int(fromUserMsg.size()), 0);
		}
		else//������ �������� �޼��� ����
		{
			cout << "fromSockNum=" << fromSockNum << "  to="<<destUser->second->GetSocket() << endl;
			const string& fromUserMsg = Data->dataKey[SECR_SEND];
			send(fromSockNum, fromUserMsg.c_str(), int(fromUserMsg.size()), 0);

			//�޴� �������� �޼��� ����
			const string& toUserMsg = format("\n\r# {}{}{}\n\r", fromUser->GetID(), Data->dataKey[SECR], msg);
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
	if (state == State::WAITING) // LOGIN ������ ����.
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

void Manager::HandleWaiting(SOCKET* targetSocket, vector<string>& words)
{
	if (words[0] == "LOGIN" && words.size() == 2 && words[1].length() > 0)
	{
		LogIn(*targetSocket, words[1]);
		cout << "�α��� �õ� *targetSocket=" << *targetSocket << " id= " << words[1];
	}
	else //�α��� ����
	{
		string msg = "** �ùٸ� ������ LOGIN [ID] �Դϴ�.\n\r";
		send(*targetSocket, msg.c_str(), int(msg.size()), 0);
	}
}

void Manager::HandleLobby(SOCKET* targetSocket, vector<string>& words)
{
	if (words[0].compare("H") == 0)
	{
		ShowAllCommand(*targetSocket);
	}
	else if (words[0].compare("US") == 0)
	{
		ShowUserList(*targetSocket);
	}
	else if (words[0].compare("LT") == 0)//��ȭ�� ��� ����
	{
		ShowRoomList(*targetSocket);
	}
	else if (words[0].compare("ST") == 0 && words.size() == 2)
	{
		ShowRoomInfo(*targetSocket, words[1]);
	}
	else if (words[0].compare("PF") == 0 && words.size() == 2)//�̿��� ���� ����
	{
		ShowUserInfo(*targetSocket, words[1]);
	}
	else if (words[0].compare("TO") == 0 && words.size() == 3)//���� ������
	{
		SendMsgToUser(*targetSocket, words[1], words[2]);
	}
	else if (words[0].compare("O") == 0 && words.size() == 3) // ��ȭ�� �����
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

void Manager::HandleRoom(SOCKET* targetSocket, vector<string>& words,const string& msgBuffer)
{
	if (words[0].compare("DEL") == 0)
	{
		DeleteRoom(*targetSocket);
	}
	else if (words[0].compare("Q") == 0)
	{
		ExitRoom(*targetSocket);
	}
	else if (words[0].compare("TO") == 0 && words.size() == 3)//���� ������
	{
		SendMsgToUser(*targetSocket, words[1], words[2]);
	}
	else
	{
		SendMsgToRoom(GetUserFromSock(*targetSocket), msgBuffer);
	}
}
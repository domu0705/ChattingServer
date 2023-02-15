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

void Manager::LogIn(SOCKET sockNum, const string& id)//��ɾ� �ȳ�
{
	auto iter = nameAry.find(id);
	if (iter != nameAry.end())//�ߺ� ���̵� ����
	{
		string msg = "** ���̵� �̹� ������Դϴ�. �ٸ� ���̵� ������ּ���.\n\r";
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
	else {
		string msg = "\r\n----------------------------------------------\n\r �ݰ����ϴ�. �ؽ�Ʈ ä�� ���� ver 0.1 �Դϴ�.\n\r �̿��� �����Ͻ� ���� ������ �Ʒ� �̸��Ϸ� ���� �ٶ��ϴ�.\n\r �����մϴ�.\n\r programmed & arranged by Minjee Kim\n\r email: minjee.kim@nm-neo.com\n\r----------------------------------------------\n\r��ɾ�ȳ�(H) ����(X)\n\r";
		send(sockNum, msg.c_str(), int(msg.size()), 0);
		cout << "User ID : " << id << endl;
		userAry[sockNum].SetID(id);
		userAry[sockNum].SetState(State::LOBBY);
		nameAry.insert(make_pair(id, &userAry[sockNum]));
	}
}

void Manager::ShowAllCommand(SOCKET sockNum)//H: ��ɾ� �ȳ�
{
	string msg = "---------------------------------------------------------------\n\rH                         ��ɾ� �ȳ�\n\rUS                        �̿��� ��� ����\n\rLT                        ��ȭ�� ��� ����\n\rST [���ȣ]               ��ȭ�� ���� ����\n\rPF [����ID]             �̿��� ���� ����\n\rDEL                         �� ����\n\rQ                         �� ������\n\rX                         ������\n\r---------------------------------------------------------------\n\r";
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowUserList(SOCKET sockNum)//US : �̿��� ��� ���� ()
{
	string header = "------------------------- �̿��� ��� -------------------------\n\r";
	string userInfo = "";
	for (auto iter = nameAry.begin(); iter != nameAry.end(); iter++)
	{
		userInfo += "�̿���: " + iter->first + "\t������: " + iter->second->GetIpAddr() + "\n\r";
		//cout << userID << "\n";
	}
	string msg = format("{}{}{}", header, userInfo,"\n\r��ɾ�ȳ�(H) ����(X)\n\r");
	send(sockNum,  +msg.c_str(), int(msg.size()), 0);
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::ShowRoomList(SOCKET sockNum)//LT : ��ȭ�� ��� ����
{
	string header = "------------------------- ��ȭ�� ��� -------------------------\n\r";
	string roomInfo = "";
	for (int i = 0;i< roomAry.size();i++)
	{
		if (roomAry[i].GetIsOpen())
		{
			roomInfo += "[" + to_string(roomAry[i].GetRoomIdx()+1) + "] (" + to_string(roomAry[i].GetCurClntNum()) + "/" + to_string(roomAry[i].GetMaxClntNum()) + ") " + roomAry[i].GetRoomName()+"\n\r";
		}
	}
	string  boundary = "----------------------------------------------------------------\n\r��ɾ�ȳ�(H) ����(X)\n\r����>";

	string msg = header + roomInfo + boundary;
	send(sockNum, +msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowRoomInfo(SOCKET sockNum,int roomIndex)//ST :L ��ȭ�� ���� ����
{
	if (roomIndex > roomAry.size()|| roomIndex <1 || !roomAry[roomIndex - 1].GetIsOpen())//���� ���ų� �̹� �����ִٸ�
	{
		string msg = "** �ش� ��ȣ�� ���� �������� �ʽ��ϴ�.\n\r";
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
	else
	{
		string msg = roomAry[roomIndex-1].GetCurRoomInfo();
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
}

void Manager::ShowUserInfo(SOCKET sockNum, const string& targetUserID)//PF: �̿��� ���� ����
{
	auto iter = nameAry.find(targetUserID);
	if (iter != nameAry.end()) 
	{
		if (iter->second->GetState() == 2)// 2 == ROOM. �濡 �ִٸ� �� �̸��� �˷��ֱ�
		{
			string msg = format("** {}���� ���� �̸� {}�� ä�ù濡 �ֽ��ϴ�.\n\r", targetUserID, iter->second->GetRoom());
			send(sockNum, msg.c_str(), int(msg.size()), 0);
		}
		else
		{
			string msg = format("** {}���� ���� �κ� �ֽ��ϴ�.\n\r", targetUserID);//iter->second.GetState()
			send(sockNum, msg.c_str(), int(msg.size()), 0);
		}
	}
	else {
		string msg = format("** {}���� ã�� �� �����ϴ�..\n\r", targetUserID);
		send(sockNum, msg.c_str(), int(msg.size()), 0);
	}
}

void Manager::MakeRoom(SOCKET sockNum, int maxClnt, const string& roomName)//O: ��ȭ�� �����
{
	if (maxClnt > 20 || maxClnt < 2)
	{
		string str = "** ��ȭ�� �ο��� 2 - 20�� ���̷� �Է����ּ���.\n\r";
		send(sockNum, str.c_str(), int(str.size()), 0);
	}
	else
	{
		cout << "MakeRoom - sockNum ID : " << sockNum << endl;
		User* user = GetUserFromSock(sockNum);

		//���ο� �� ����
		Room newRoom;
		newRoom.SetRoom(roomIdx, roomName, user->GetID(), GetCurTime(), maxClnt);
		roomAry.push_back(newRoom);
		roomAry[roomIdx].EnterUser(user, GetCurTime());

		//Ŭ�� �˸�������
		string str = "** ��ȭ���� �����Ǿ����ϴ�.\n\r";

		send(sockNum, str.c_str(), int(str.size()), 0);
		string msg = format("** {}���� �����̽��ϴ�. (�����ο� {}/{})\n\r", user->GetID(), roomAry[roomIdx].GetCurClntNum(), roomAry[roomIdx].GetMaxClntNum());
		roomAry[roomIdx].SendMsgToRoom(msg);
		++roomIdx;
	}
}

void Manager::JoinRoom(SOCKET sockNum, int roomNum)//��ȭ�� �����ϱ�
{
	if (roomNum > roomAry.size() || roomNum < 1)
	{
		string msg = "** �ش� ���� �������� �ʽ��ϴ�. \n\r��ɾ�ȳ�(H) ����(X)\n\r";
		send(sockNum, msg.c_str(), int(msg.size()), 0);
		
	}
	else //�� ��ȣ�� ����
	{
		if (roomAry[roomNum - 1].GetMaxClntNum() <= roomAry[roomNum - 1].GetCurClntNum())
		{
			string msg = "** �ο��� �� ���� ������ �� �����ϴ�.\n\r��ɾ�ȳ�(H) ����(X)\n\r";
			send(sockNum, msg.c_str(), int(msg.size()), 0);
		}
		else
		{
			User* user = GetUserFromSock(sockNum);
			roomAry[roomNum-1].EnterUser(user, GetCurTime());

			string msg = format("** {}���� �����̽��ϴ�. (�����ο� {}/{})\n\r", user->GetID(), roomAry[roomNum - 1].GetCurClntNum(), roomAry[roomNum - 1].GetMaxClntNum());
			roomAry[roomNum - 1].SendMsgToRoom(msg);
		}
	}
	
}

void Manager::ExitSystem(SOCKET sockNum)//������
{
	User* user = GetUserFromSock(sockNum);
	user->SetState(LOGOUT);
	string msg = "\r\n----------------------------------------------\n\r �̿����ּż� �����մϴ�.\n\r ���� �Ϸ� �ູ�Ͻñ� �ٶ��ϴ� :) \n\r programmed & arranged by Minjee Kim\n\r email: minjee.kim@nm-neo.com\n\r----------------------------------------------\n\r��ɾ�ȳ�(H) ����(X)\n\r";
	send(sockNum, msg.c_str(), int(msg.size()), 0);
	nameAry.erase(user->GetID());

}

void Manager::NotExistingCommend(SOCKET sockNum)//������
{
	string msg = "\n\r�������� �ʴ� ����Դϴ�. \n\r��ɾ�ȳ�(H) ����(X)\n\r";
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::DeleteRoom(SOCKET sockNum)//Ȯ��
{
	User* user = GetUserFromSock(sockNum);
	int curRoomNum = user->GetRoomNum();
	//int curClntNum = roomAry[curRoomNum]->GetCurClntNum();
	set<User*> roomUserAry = roomAry[curRoomNum].GetUserAry();

	roomAry[curRoomNum].SetMaxClntNum(0);//�� ����
	roomAry[curRoomNum].CloseRoom();
	//curClent �� 0�ž� �� ���� userAr

	string msg = "\n\r�ش� ���� ���ĵǾ����ϴ�. �κ�� �̵��մϴ�.\n\r��ɾ�ȳ�(H) ����(X)\n\r";

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

void Manager::SendMsgToRoom(User* user, const string& msg) // room���� �� ���η� ä�� ������
{
	string msgInfo = format("{} > {}\n\r", user->GetID(), msg);
	roomAry[user->GetRoomNum()].SendMsgToRoom(msgInfo);
}

void Manager::SendMsgToUser(SOCKET fromSockNum, const string& toUser, const string& msg) //���� ������
{
	auto destUser = nameAry.find(toUser);
	if (destUser != nameAry.end()) {
		User* fromUser = GetUserFromSock(fromSockNum);

		//�ڱ� �ڽſ��� �������� Ȯ��
		if (fromUser->GetID().compare(toUser) == 0)
		{
			string fromUserMsg = "\n\r** �ڱ� �ڽſ��Դ� ���� �� �����ϴ�.\n\r";
			send(fromSockNum, fromUserMsg.c_str(), int(fromUserMsg.size()), 0);
		}
		else//������ �������� �޼��� ����
		{
			string fromUserMsg = "\n\r** ������ ���½��ϴ�.\n\r";
			send(fromSockNum, fromUserMsg.c_str(), int(fromUserMsg.size()), 0);

			//�޴� �������� �޼��� ����
			string toUserMsg = format("\n\r# {}���� ���� ==> {}\n\r", fromUser->GetID(), msg);
			send(destUser->second->GetSocket(), toUserMsg.c_str(), int(toUserMsg.size()), 0);
		}
	}
	else {
		string warnMsg = "\n\r** �ش� ������ �������� �ʽ��ϴ�.\n\r";
		send(fromSockNum, warnMsg.c_str(), int(warnMsg.size()), 0);
	}
}

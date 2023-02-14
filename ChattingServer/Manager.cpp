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

void Manager::LogIn(SOCKET sockNum, string id)//��ɾ� �ȳ�
{
	string msg = "\r\n----------------------------------------------\n\r �ݰ����ϴ�. �ؽ�Ʈ ä�� ���� ver 0.1 �Դϴ�.\n\r �̿��� �����Ͻ� ���� ������ �Ʒ� �̸��Ϸ� ���� �ٶ��ϴ�.\n\r �����մϴ�.\n\r programmed & arranged by Minjee Kim\n\r email: minjee.kim@nm-neo.com\n\r----------------------------------------------\n\r��ɾ�ȳ�(H) ����(X)\n\r";
	send(sockNum, msg.c_str(), int(msg.size()), 0);
	cout << "User ID : " << id << endl;
	userAry[sockNum].SetID(id);
	userAry[sockNum].SetState(State::LOBBY);
	nameAry.insert(make_pair(id, userAry[sockNum]));
}

void Manager::ShowAllCommand(SOCKET sockNum)//H: ��ɾ� �ȳ�
{
	string msg = "---------------------------------------------------------------\n\rH                         ��ɾ� �ȳ�\n\rUS                        �̿��� ��� ����\n\rLT                        ��ȭ�� ��� ����\n\rST [���ȣ]               ��ȭ�� ���� ����\n\rPF [����ID]             �̿��� ���� ����\n\rX                         ������\n\r---------------------------------------------------------------\n\r";
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowUserList(SOCKET sockNum)//US : �̿��� ��� ���� ()
{
	for (auto iter = nameAry.begin(); iter != nameAry.end(); iter++)
	{
		string header = "------------------------- �̿��� ��� -------------------------\n\r";
		string userID = header+"�̿���: " + iter->first + "\t������: "+ to_string(sockNum);
		cout << userID << "\n";
		send(sockNum, userID.c_str(), int(userID.size()), 0);
	}
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::ShowRoomList(SOCKET sockNum)//LT : ��ȭ�� ��� ����
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::ShowRoomInfo(SOCKET sockNum,int _roomIdx)//ST :L ��ȭ�� ���� ����
{
	string msg = roomAry[_roomIdx-1].GetCurRoomInfo();
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowUserInfo(SOCKET sockNum)//PF: �̿��� ���� ����
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::TO(SOCKET sockNum)//���� ������
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::MakeRoom(SOCKET sockNum, int maxClnt, string roomName)//O: ��ȭ�� �����
{
	cout << "MakeRoom - sockNum ID : " << sockNum << endl;
	User* user = GetUserFromSock(sockNum);

	//���ο� �� ����
	Room newRoom;
	newRoom.SetRoom(roomIdx, roomName, user->GetID(), GetCurTime(), maxClnt);
	roomAry.push_back(newRoom);
	roomAry[roomIdx].EnterUser(user, GetCurTime());

	//Ŭ�� �˸�������
	string str = "** ��ȭ���� �����Ǿ����ϴ�.";
	send(sockNum, str.c_str(), int(str.size()), 0);
}

void Manager::J(SOCKET sockNum)//��ȭ�� �����ϱ�
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::X(SOCKET sockNum)//������
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::NotExistingCommend(SOCKET sockNum)//������
{
	string msg = "\n\r�������� �ʴ� ����Դϴ�. \n\r��ɾ�ȳ�(H) ����(X)\n\r";
	send(sockNum, msg.c_str(), int(msg.size()), 0);
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

/* tm ����ü ����
struct tm {
   int tm_sec;         // ��,  range 0 to 59
   int tm_min;         // ��, range 0 to 59
   int tm_hour;        // �ð�, range 0 to 23
   int tm_mday;        // ��, range 1 to 31
   int tm_mon;         // ��, range 0 to 11
   int tm_year;        // 1900�� ������ ��
   int tm_wday;        // ����, range ��(0) to ��(6)
   int tm_yday;        // 1�� �� ��� ��, range 0 to 365
   int tm_isdst;       // ����Ÿ�� �ǽ� ���� (���, 0, ����)
};
*/
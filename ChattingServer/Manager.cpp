#include "Manager.h"

Manager::Manager()
{
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
	string msg = "\r\n----------------------------------------------\n\r �ݰ����ϴ�. \
					�ؽ�Ʈ ä�� ���� ver 0.1 �Դϴ�.\n\r �̿��� �����Ͻ� ���� ������ �Ʒ� �̸��Ϸ� ���� �ٶ��ϴ�.\n\r �����մϴ�.\n\r programmed & arranged by Minjee Kim\n\r email: minjee.kim@nm-neo.com\n\r----------------------------------------------\n\r��ɾ�ȳ�(H) ����(X)\n\r";
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
	//string msg = Data.dataAry[Sentance_H];
	//send(sockNum, msg.c_str(), int(msg.size()), 0);
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

void Manager::ShowRoomInfo(SOCKET sockNum)//ST :L ��ȭ�� ���� ����
{
	cout << "sockNum ID : " << sockNum << endl;
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
	Room newRoom;
	newRoom.SetRoom(user, roomName, "������", "1��", maxClnt);
	roomAry.push_back(newRoom);
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
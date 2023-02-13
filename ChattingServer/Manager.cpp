#include "Manager.h"

Manager::Manager()
{

}


User Manager::GetUserFromSock(SOCKET sockNum)
{
	User user = userAry[sockNum];
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

void Manager::H(SOCKET sockNum)//��ɾ� �ȳ�
{
	string msg = "---------------------------------------------------------------\n\rH                         ��ɾ� �ȳ�\n\rUS                        �̿��� ��� ����\n\rLT                        ��ȭ�� ��� ����\n\rST [���ȣ]               ��ȭ�� ���� ����\n\rPF [����ID]             �̿��� ���� ����\n\rX                         ������\n\r---------------------------------------------------------------\n\r";
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::US(SOCKET sockNum)//�̿��� ��� ����
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::LT(SOCKET sockNum)//��ȭ�� ��� ����
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::ST(SOCKET sockNum)//��ȭ�� ���� ����
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::PF(SOCKET sockNum)//�̿��� ���� ����
{
	cout << "sockNum ID : " << sockNum << endl;
}
void Manager::TO(SOCKET sockNum)//���� ������
{
	cout << "sockNum ID : " << sockNum << endl;
}
void Manager::O(SOCKET sockNum)//��ȭ�� �����
{
	cout << "sockNum ID : " << sockNum << endl;
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
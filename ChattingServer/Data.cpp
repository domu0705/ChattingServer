#include "Data.h"

Data::Data()
{
	GenerateData();
}

void Data::GenerateData()
{
	dataKey[Sentance_LT_HEADER]="------------------------- ��ȭ�� ���-------------------------- \n\r" ;
	
	dataKey[Sentance_LT_BOUNDARY]="---------------------------------------------------------------\n\r��ɾ�ȳ�(H) ����(X)\n\r����>";
	dataKey[LOGIN_ID_DUP]="**���̵� �̹� ������Դϴ�.�ٸ� ���̵� ������ּ���.\n\r";
	dataKey[LOGIN_HI]="\r\n----------------------------------------------\n\r �ݰ����ϴ�. �ؽ�Ʈ ä�� ���� ver 0.1 �Դϴ�.\n\r �̿��� �����Ͻ� ���� ������ �Ʒ� �̸��Ϸ� ���� �ٶ��ϴ�.\n\r �����մϴ�.\n\r programmed & arranged by Minjee Kim\n\r email: minjee.kim@nm-neo.com\n\r----------------------------------------------\n\r��ɾ�ȳ�(H) ����(X)\n\r";
	dataKey[COMMAND_HELP]="--------------------------------------------------------------- \n\rH                         ��ɾ� �ȳ�\n\rUS                        �̿��� ��� ����\n\rLT                        ��ȭ�� ��� ����\n\rST[���ȣ]               ��ȭ�� ���� ����\n\rPF[����ID]             �̿��� ���� ����\n\rDEL                         �� ����\n\rQ                         �� ������\n\rX                         ������\n\r--------------------------------------------------------------- \n\r��ɾ�ȳ�(H) ����(X)\n\r";
	dataKey[USERLIST_HEADER]="------------------------- �̿��� ��� -------------------------\n\r";
	dataKey[HELP_LITTLE]="\n\r��ɾ�ȳ�(H) ����(X)\n\r";
	dataKey[ROOM_NUM_NOT_EXIST]="** �ش� ��ȣ�� ���� �������� �ʽ��ϴ�.\n\r";
	dataKey[ROOM_MAX_CLNT]="** ��ȭ�� �ο��� 2 - 20�� ���̷� �Է����ּ���.\n\r";
	dataKey[ROOM_GEN] = "** ��ȭ���� �����Ǿ����ϴ�.\n\r";
	dataKey[ROOM_NOT_EXIST] = "** �ش� ���� �������� �ʽ��ϴ�. ";
	dataKey[ROOM_MAX] = "** �ο��� �� ���� ������ �� �����ϴ�. ";
	dataKey[IN_LOBBY] = "���� ���� �κ� �ֽ��ϴ�.\n\r";
	dataKey[USER_INFO_NON] = "���� ã�� �� �����ϴ�..\n\r";
	dataKey[THX] = "\r\n----------------------------------------------\n\r �̿����ּż� �����մϴ�.\n\r ���� �Ϸ� �ູ�Ͻñ� �ٶ��ϴ� :) \n\r programmed & arranged by Minjee Kim\n\r email: minjee.kim@nm-neo.com\n\r----------------------------------------------\n\r";
	dataKey[COMM_NOT_EXIST] = "\n\r�������� �ʴ� ����Դϴ�. \n\r��ɾ�ȳ�(H) ����(X)\n\r";
	dataKey[ROOM_DEL] = "\n\r�ش� ���� ���ĵǾ����ϴ�. �κ�� �̵��մϴ�.\n\r��ɾ�ȳ�(H) ����(X)\n\r";
	dataKey[MYSELF_NO] = "\n\r** �ڱ� �ڽſ��Դ� ���� �� �����ϴ�.\n\r";
	dataKey[SECR_SEND] = "\n\r** ������ ���½��ϴ�.\n\r";
	dataKey[SECR] = "���� ���� ==> ";
	dataKey[USER_NOT_EXIST] = "\n\r** �ش� ������ �������� �ʽ��ϴ�.\n\r";
	dataKey[OTHER_ENTERED] = "���� �����̽��ϴ�. (�����ο�";
	dataKey[ROOM_SENT_1] = "���� ���� �̸� ";
	dataKey[ROOM_SENT_2] = "�� ä�ù濡 �ֽ��ϴ�.\n\r";
	dataKey[Sentance_LT_HEADER] = "------------------------- ��ȭ�� ���-------------------------- \n\r";
	dataKey[ROOM_LIST_INFO]= "------------------------- ��ȭ�� ���� -------------------------\n\r";
	dataKey[ROOM_EXIT] = "���� ���� �����̽��ϴ�.\n\r";
	/*
	dataKey.push_back("");
	dataKey.push_back("");
	dataKey.push_back("");
	dataKey.push_back("");
	dataKey.push_back("");
	dataKey.push_back("");
	*/
}
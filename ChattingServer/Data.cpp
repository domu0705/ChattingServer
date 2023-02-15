#include "Data.h"

Data::Data()
{
	GenerateData();
}

void Data::GenerateData()
{
	dataKey[Sentance_LT_HEADER]="------------------------- 대화방 목록-------------------------- \n\r" ;
	
	dataKey[Sentance_LT_BOUNDARY]="---------------------------------------------------------------\n\r명령어안내(H) 종료(X)\n\r선택>";
	dataKey[LOGIN_ID_DUP]="**아이디를 이미 사용중입니다.다른 아이디를 사용해주세요.\n\r";
	dataKey[LOGIN_HI]="\r\n----------------------------------------------\n\r 반갑습니다. 텍스트 채팅 서버 ver 0.1 입니다.\n\r 이용중 불편하신 점이 있으면 아래 이메일로 문의 바랍니다.\n\r 감사합니다.\n\r programmed & arranged by Minjee Kim\n\r email: minjee.kim@nm-neo.com\n\r----------------------------------------------\n\r명령어안내(H) 종료(X)\n\r";
	dataKey[COMMAND_HELP]="--------------------------------------------------------------- \n\rH                         명령어 안내\n\rUS                        이용자 목록 보기\n\rLT                        대화방 목록 보기\n\rST[방번호]               대화방 정보 보기\n\rPF[상대방ID]             이용자 정보 보기\n\rDEL                         방 폭파\n\rQ                         방 나가기\n\rX                         끝내기\n\r--------------------------------------------------------------- \n\r명령어안내(H) 종료(X)\n\r";
	dataKey[USERLIST_HEADER]="------------------------- 이용자 목록 -------------------------\n\r";
	dataKey[HELP_LITTLE]="\n\r명령어안내(H) 종료(X)\n\r";
	dataKey[ROOM_NUM_NOT_EXIST]="** 해당 번호의 방은 존재하지 않습니다.\n\r";
	dataKey[ROOM_MAX_CLNT]="** 대화방 인원을 2 - 20명 사이로 입력해주세요.\n\r";
	dataKey[ROOM_GEN] = "** 대화방이 개설되었습니다.\n\r";
	dataKey[ROOM_NOT_EXIST] = "** 해당 방은 존재하지 않습니다. ";
	dataKey[ROOM_MAX] = "** 인원이 꽉 차서 참여할 수 없습니다. ";
	dataKey[IN_LOBBY] = "님은 현재 로비에 있습니다.\n\r";
	dataKey[USER_INFO_NON] = "님을 찾을 수 없습니다..\n\r";
	dataKey[THX] = "\r\n----------------------------------------------\n\r 이용해주셔서 감사합니다.\n\r 오늘 하루 행복하시길 바랍니다 :) \n\r programmed & arranged by Minjee Kim\n\r email: minjee.kim@nm-neo.com\n\r----------------------------------------------\n\r";
	dataKey[COMM_NOT_EXIST] = "\n\r존재하지 않는 명령입니다. \n\r명령어안내(H) 종료(X)\n\r";
	dataKey[ROOM_DEL] = "\n\r해당 방은 폭파되었습니다. 로비로 이동합니다.\n\r명령어안내(H) 종료(X)\n\r";
	dataKey[MYSELF_NO] = "\n\r** 자기 자신에게는 보낼 수 없습니다.\n\r";
	dataKey[SECR_SEND] = "\n\r** 쪽지를 보냈습니다.\n\r";
	dataKey[SECR] = "님의 쪽지 ==> ";
	dataKey[USER_NOT_EXIST] = "\n\r** 해당 유저는 존재하지 않습니다.\n\r";
	dataKey[OTHER_ENTERED] = "님이 들어오셨습니다. (현재인원";
	dataKey[ROOM_SENT_1] = "님은 현재 이름 ";
	dataKey[ROOM_SENT_2] = "인 채팅방에 있습니다.\n\r";
	dataKey[Sentance_LT_HEADER] = "------------------------- 대화방 목록-------------------------- \n\r";
	dataKey[ROOM_LIST_INFO]= "------------------------- 대화방 정보 -------------------------\n\r";
	dataKey[ROOM_EXIT] = "님이 방을 나가셨습니다.\n\r";
	/*
	dataKey.push_back("");
	dataKey.push_back("");
	dataKey.push_back("");
	dataKey.push_back("");
	dataKey.push_back("");
	dataKey.push_back("");
	*/
}
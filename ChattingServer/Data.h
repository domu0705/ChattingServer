// -----------------------------------------------------------------------------------
//  문자열 정보 저장
// -----------------------------------------------------------------------------------
#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

enum Sentance // 사용 예시: string msg = Data.dataKey[COMMAND_HELP];
{
	Sentance_LT_HEADER,
	Sentance_LT_BOUNDARY,
	LOGIN_ID_DUP,
	LOGIN_HI,
	COMMAND_HELP,
	USERLIST_HEADER,
	HELP_LITTLE,//"\n\r명령어안내(H) 종료(X)\n\r"
	ROOM_NUM_NOT_EXIST,
	ROOM_MAX_CLNT,
	ROOM_GEN,
	ROOM_NOT_EXIST,
	ROOM_MAX,
	IN_LOBBY,
	USER_INFO_NON,
	THX,
	COMM_NOT_EXIST,
	ROOM_DEL,
	MYSELF_NO,
	SECR_SEND,
	SECR,
	USER_NOT_EXIST,
	OTHER_ENTERED,
	ROOM_SENT_1,
	ROOM_SENT_2,
	ROOM_LIST_INFO,
	ROOM_EXIT,
	HELLO
};

class Data
{
private:
public:
	static Data& GetInstance() {
		static Data s;
		return s;
	}
	map<int,string> dataKey;

	Data();

	void GenerateData();

};

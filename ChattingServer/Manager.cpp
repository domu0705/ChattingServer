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

void Manager::LogIn(SOCKET sockNum, string id)//명령어 안내
{
	string msg = "\r\n----------------------------------------------\n\r 반갑습니다. 텍스트 채팅 서버 ver 0.1 입니다.\n\r 이용중 불편하신 점이 있으면 아래 이메일로 문의 바랍니다.\n\r 감사합니다.\n\r programmed & arranged by Minjee Kim\n\r email: minjee.kim@nm-neo.com\n\r----------------------------------------------\n\r명령어안내(H) 종료(X)\n\r";
	send(sockNum, msg.c_str(), int(msg.size()), 0);
	cout << "User ID : " << id << endl;
	userAry[sockNum].SetID(id);
	userAry[sockNum].SetState(State::LOBBY);
	nameAry.insert(make_pair(id, userAry[sockNum]));
}

void Manager::ShowAllCommand(SOCKET sockNum)//H: 명령어 안내
{
	string msg = "---------------------------------------------------------------\n\rH                         명령어 안내\n\rUS                        이용자 목록 보기\n\rLT                        대화방 목록 보기\n\rST [방번호]               대화방 정보 보기\n\rPF [상대방ID]             이용자 정보 보기\n\rX                         끝내기\n\r---------------------------------------------------------------\n\r";
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowUserList(SOCKET sockNum)//US : 이용자 목록 보기 ()
{
	for (auto iter = nameAry.begin(); iter != nameAry.end(); iter++)
	{
		string header = "------------------------- 이용자 목록 -------------------------\n\r";
		string userID = header+"이용자: " + iter->first + "\t접속지: "+ to_string(sockNum);
		cout << userID << "\n";
		send(sockNum, userID.c_str(), int(userID.size()), 0);
	}
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::ShowRoomList(SOCKET sockNum)//LT : 대화방 목록 보기
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::ShowRoomInfo(SOCKET sockNum,int _roomIdx)//ST :L 대화방 정보 보기
{
	string msg = roomAry[_roomIdx-1].GetCurRoomInfo();
	send(sockNum, msg.c_str(), int(msg.size()), 0);
}

void Manager::ShowUserInfo(SOCKET sockNum)//PF: 이용자 정보 보기
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::TO(SOCKET sockNum)//쪽지 보내기
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::MakeRoom(SOCKET sockNum, int maxClnt, string roomName)//O: 대화방 만들기
{
	cout << "MakeRoom - sockNum ID : " << sockNum << endl;
	User* user = GetUserFromSock(sockNum);

	//새로운 방 생성
	Room newRoom;
	newRoom.SetRoom(roomIdx, roomName, user->GetID(), GetCurTime(), maxClnt);
	roomAry.push_back(newRoom);
	roomAry[roomIdx].EnterUser(user, GetCurTime());

	//클라 알림보내기
	string str = "** 대화방이 개설되었습니다.";
	send(sockNum, str.c_str(), int(str.size()), 0);
}

void Manager::J(SOCKET sockNum)//대화방 참여하기
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::X(SOCKET sockNum)//끝내기
{
	cout << "sockNum ID : " << sockNum << endl;
}

void Manager::NotExistingCommend(SOCKET sockNum)//끝내기
{
	string msg = "\n\r존재하지 않는 명령입니다. \n\r명령어안내(H) 종료(X)\n\r";
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

/* tm 구조체 내부
struct tm {
   int tm_sec;         // 초,  range 0 to 59
   int tm_min;         // 분, range 0 to 59
   int tm_hour;        // 시간, range 0 to 23
   int tm_mday;        // 일, range 1 to 31
   int tm_mon;         // 월, range 0 to 11
   int tm_year;        // 1900년 부터의 년
   int tm_wday;        // 요일, range 일(0) to 토(6)
   int tm_yday;        // 1년 중 경과 일, range 0 to 365
   int tm_isdst;       // 섬머타임 실시 여부 (양수, 0, 음수)
};
*/
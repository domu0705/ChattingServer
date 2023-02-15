// -----------------------------------------------------------------------------------
//  서버와 클라이언트의 연결 및 입출력을 관리
// -----------------------------------------------------------------------------------
#include "USER.h"
#include "Manager.h"
//#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#include <format>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>


vector<string> split(string str, char Delimiter) {
	istringstream ss(str);      
	string buffer;                 
	vector<string> result;

	while (getline(ss, buffer, Delimiter)) {
		result.push_back(buffer);         
	}
	return result;
}

int main()
{
	Manager& manager = Manager::GetInstance();
	WSADATA	wsaData;
	SOCKET servSock, clntSock;
	SOCKADDR_IN servAddr, clntAddr;

	u_short portNum = 2222; 

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
		cout << "WSAStartup() error!";

	servSock = socket(PF_INET, SOCK_STREAM, 0); 
	if (servSock == INVALID_SOCKET)
		cout << "socket() error";

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET; 
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(portNum);

	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		cout << "bind() error" << endl;
	else
		cout << "bind() success" << endl;

	if (listen(servSock, 5) == SOCKET_ERROR)
		cout << "listen() error";
	else
		cout << "listen() success" << endl;

	//SELECT 사용환경 설정
	TIMEVAL timeout;
	fd_set reads, cpyReads;//파일 디스크립터 배열(0,1로 표현되는 bit단위)

	int adrSz;
	int strLen, result;

	FD_ZERO(&reads);
	FD_SET(servSock, &reads);
	SOCKET* targetSocket;
	//string buf;
	char c;

	while (true)
	{
		cpyReads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		if ((result = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
		{
			break;
		}

		if (result == 0) 
		{
			continue;
		}

		for (int i = 0; i < int(reads.fd_count); ++i)//select가 1 이상 반환됐을 때 실행됨
		{
			targetSocket = &reads.fd_array[i];
			if (FD_ISSET(*targetSocket, &cpyReads)) // FD_ISSET로 상태변화가 있었던(수신된 데이터가 있는 소켓의)파일 디스크립터를 찾음
			{
				if (*targetSocket == servSock) // 서버 소켓에서 변화가 있었는지 확인. 맞다면 연결 요청을 수락하는 과정 진행.(connection request) 클라와 연결
				{
					adrSz = sizeof(clntAddr);
					clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &adrSz);
					
					string userAddr = format("{}:{}", inet_ntoa(clntAddr.sin_addr), ntohs(clntAddr.sin_port));
					User user(clntSock, userAddr);//유저와 소켓 연결 정보 저장
					manager.SetUserToUserSockAry(clntSock, user);

					FD_SET(clntSock, &reads);

					string msg = "* * 안녕하세요.텍스트 채팅 서버 ver 0.1입니다.\n\r* * 로그인 명령어(LOGIN)를 사용해주세요 !\n\r ";
					send(clntSock, msg.c_str(), int(msg.size()), 0);   // c_str 는 string을 char * 로 변환
				}
				else    // 상태가 변한 소켓이 서버소켓이 아님.  즉 수신할 데이터가 있음. (read message)
				{	
					strLen = recv(*targetSocket, &c, sizeof(char), 0);
					User* user = manager.GetUserFromSock(*targetSocket);

					if (strLen == 0 )    // close request!
					{
						FD_CLR(*targetSocket, &reads);
						closesocket(cpyReads.fd_array[i]);
					}
					else if (c == '\n') // 클라가 엔터를 입력했다면 여기로 가서 답을 줘야할 듯
					{
						cout << "???" << endl;
						string msgBuf = user->buffer.substr(0, user->buffer.length() - 1);// 뒤에 자동으로 오는 \r을 제거
						vector<string> word = split(msgBuf, ' ');

						if (word.size() == 0) //아무것도 입력안하고 엔터만 침
						{
							user->buffer.clear();
							continue;
						}
						if (word[0].compare("X") == 0)//X : 종료 요청함
						{
							manager.ExitSystem(*targetSocket);
							FD_CLR(*targetSocket, &reads);
							closesocket(cpyReads.fd_array[i]);
							cout << "closed client : " << cpyReads.fd_array[i] << endl;
							continue;
						}

						if (manager.userAry[*targetSocket].GetState() == State::WAITING) // LOGIN 이전의 상태. 로그인해야함
						{
							if (word[0] == "LOGIN" && word.size() == 2 && word[1].length() > 0)//word.size() > 1 && 
							{
								manager.LogIn(*targetSocket, word[1]);
							}
							else //로그인 실패
							{
								string msg = "** 올바른 사용법은 LOGIN [ID] 입니다.\n\r";
								send(*targetSocket, msg.c_str(), int(msg.size()), 0);
							}
						}
						else if (manager.userAry[*targetSocket].GetState() == State::LOBBY)
						{
							if (word[0].compare("H") == 0)
							{
								manager.ShowAllCommand(*targetSocket);
							}
							else if (word[0].compare("US") == 0) // compare() : 같으면 0 , 다르면 -1 리턴
							{
								manager.ShowUserList(*targetSocket);
							}
							else if (word[0].compare("LT") == 0)//대화방 목록 보기
							{
								manager.ShowRoomList(*targetSocket);
							}
							else if (word[0].compare("ST") == 0 && word.size() == 2)
							{
								manager.ShowRoomInfo(*targetSocket, word[1]);
							}
							else if (word[0].compare("PF") == 0 && word.size() == 2)//PF: 이용자 정보 보기
							{
								manager.ShowUserInfo(*targetSocket, word[1]);
							}
							else if (word[0].compare("TO") == 0 && word.size() == 3)//쪽지 보내기
							{
								manager.SendMsgToUser(*targetSocket, word[1], word[2]);
							}
							else if (word[0].compare("O") == 0 && word.size() == 3) // 대화방 만들기
							{
								manager.MakeRoom(*targetSocket, word[1], word[2]);
							}
							else if (word[0].compare("J") == 0 && word.size() == 2)
							{
								manager.JoinRoom(*targetSocket, word[1]);
							}
							else if (word[0].compare("X") == 0)
							{
								manager.ExitSystem(*targetSocket);
							}
							else
							{
								manager.NotExistingCommend(reads.fd_array[i]);
							}
						}
						else if (manager.userAry[*targetSocket].GetState() == State::ROOM)
						{
							if (word[0].compare("DEL") == 0)
							{
								manager.DeleteRoom(*targetSocket);
							}
							else if (word[0].compare("Q") == 0)
							{
								manager.ExitRoom(*targetSocket);
							}
							else if (word[0].compare("TO") == 0 && word.size() == 3)//쪽지 보내기
							{
								manager.SendMsgToUser(*targetSocket, word[1], word[2]);
							}
							else
							{
								manager.SendMsgToRoom(manager.GetUserFromSock(*targetSocket), msgBuf);
							}
						}
						else{}
						user->buffer.clear();
					}
					else // 클라가 문자를 입력했다면 (수신할 데이터가 문자열인 경우)
					{
						user->buffer +=c;
						cout << "user->buffer=" << user->buffer << endl;
					}
				}
			}
		}
	}
	closesocket(servSock);
	WSACleanup();
	return 0;
}
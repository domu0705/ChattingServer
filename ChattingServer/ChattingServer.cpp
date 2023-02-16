// -----------------------------------------------------------------------------------
//  서버와 클라이언트의 연결 및 입출력을 관리
// -----------------------------------------------------------------------------------
#include "Define.h"
#include "USER.h"
#include "Manager.h"
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

	if (WSAStartup(MAKEWORD(Define::B_LOW, Define::B_HIGH), &wsaData) != 0)
		cout << "WSAStartup() error!";

	servSock = socket(PF_INET, SOCK_STREAM, 0); 
	if (servSock == INVALID_SOCKET)
		cout << "socket() error";

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET; 
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons( Define::PORT_NUMBER );

	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		cout << "bind() error" << endl;
	else 
		cout << "bind() success"<<endl;

	if (listen(servSock, Define::BACK_LOG) == SOCKET_ERROR)
		cout << "listen() error";
	else 
		cout << "listen() success" << endl;


	//SELECT 사용환경 설정
	TIMEVAL timeout;
	fd_set reads, cpyReads;

	int adrSz;
	int strLen, result;

	FD_ZERO(&reads);
	FD_SET(servSock, &reads);
	SOCKET* targetSocket;
	char c;

	while (true)
	{
		cpyReads = reads;
		timeout.tv_sec = Define::TV_SEC;
		timeout.tv_usec = Define::TV_USEC;

		if ((result = select(Define::FD_NUM, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
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
			if (!FD_ISSET(*targetSocket, &cpyReads))
			{
				continue;
			}
			if (*targetSocket == servSock) // 서버 소켓의 변화 확인(맞다면 연결 요청을 수락)
			{
				adrSz = sizeof(clntAddr);
				clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &adrSz);
					
				string userAddr = format("{}:{}", inet_ntoa(clntAddr.sin_addr), ntohs(clntAddr.sin_port));
				User user(clntSock, userAddr);//유저와 소켓 연결 정보 저장
				manager.SetUserToUserSockAry(clntSock, user);

				FD_SET(clntSock, &reads);

				string msg = "* * 안녕하세요.텍스트 채팅 서버 ver 0.1입니다.\n\r* * 로그인 명령어(LOGIN)를 사용해주세요 !\n\r ";
				send(clntSock, msg.c_str(), int(msg.size()), 0); 
			}
			else    // 즉 수신할 데이터가 있음. (read message)
			{	
				strLen = recv(*targetSocket, &c, sizeof(char), 0);
				User* user = manager.GetUserFromSock(*targetSocket);

				if (strLen == 0 )    // close request!
				{
					FD_CLR(*targetSocket, &reads);
					closesocket(cpyReads.fd_array[i]);
				}
				else if (c == '\n') 
				{
					string msgBuf = user->buffer.substr(0, user->buffer.length() - 1);
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
						cout << "Closed client : " << cpyReads.fd_array[i] << endl;
						continue;
					}

					int curState = manager.userAry[*targetSocket].GetState();
					if (curState == State::WAITING) // LOGIN 이전의 상태.
					{
						if (word[0] == "LOGIN" && word.size() == 2 && word[1].length() > 0)
						{
							manager.LogIn(*targetSocket, word[1]);
						}
						else //로그인 실패
						{
							string msg = "** 올바른 사용법은 LOGIN [ID] 입니다.\n\r";
							send(*targetSocket, msg.c_str(), int(msg.size()), 0);
						}
					}
					else if (curState == State::LOBBY)
					{
						if (word[0].compare("H") == 0)
						{
							manager.ShowAllCommand(*targetSocket);
						}
						else if (word[0].compare("US") == 0)
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
						else if (word[0].compare("PF") == 0 && word.size() == 2)//이용자 정보 보기
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
					else if (curState == State::ROOM)
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
					user->buffer.clear();
				}
				else 
				{
					user->buffer +=c;
				}
			}
			
		}
	}
	closesocket(servSock);
	WSACleanup();
	return 0;
}
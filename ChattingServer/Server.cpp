#include "Server.h"


Server::Server() 
{
	manager = &Manager::GetInstance();
}


void Server::PrepareServer()
{
	if (WSAStartup(MAKEWORD(Define::B_LOW, Define::B_HIGH), &wsaData) != 0)
		cout << "WSAStartup() error!";

	servSock = socket(PF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET)
		cout << "socket() error";

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(Define::PORT_NUMBER);

	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		cout << "bind() error" << endl;
	else
		cout << "bind() success" << endl;

	if (listen(servSock, Define::BACK_LOG) == SOCKET_ERROR)
		cout << "listen() error";
	else
		cout << "listen() success" << endl;
}

void Server::StartConn()
{
	FD_ZERO(&reads);
	FD_SET(servSock, &reads);

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
				User* user = new User(clntSock, userAddr);
				manager->SetUserToUserSockAry(clntSock, user);

				FD_SET(clntSock, &reads);

				string msg = "* * 안녕하세요.텍스트 채팅 서버 ver 0.1입니다.\n\r* * 로그인 명령어(LOGIN)를 사용해주세요 !\n\r ";
				send(clntSock, msg.c_str(), int(msg.size()), 0);
			}
			else    // 수신할 데이터가 있음. (read message)
			{
				strLen = recv(*targetSocket, &c, sizeof(char), 0);
				User* user = manager->GetUserFromSock(*targetSocket);

				if (strLen == 0)    // close request!
				{
					manager->DisconnectUser(*targetSocket);
					FD_CLR(*targetSocket, &reads);
					closesocket(cpyReads.fd_array[i]);
				}
				else if (c == '\n')
				{
					const string& msgBuf = user->buffer.substr(0, user->buffer.length() - 1);
					vector<string> word = Split(msgBuf, ' ');

					if (word.size() == 0) //엔터만 입력받음
					{
						user->buffer.clear();
						continue;
					}
					if (word[0].compare("X") == 0)//X : 종료 요청함
					{
						manager->DisconnectUser(*targetSocket);
						FD_CLR(*targetSocket, &reads);
						closesocket(cpyReads.fd_array[i]);
						cout << "Closed client : " << cpyReads.fd_array[i] << endl;
						continue;
					}

					auto& userArray = manager->GetUserAry();
					int curState = userArray[*targetSocket]->GetState();
					manager->HandleState(curState, targetSocket, word, msgBuf);

					user->buffer.clear();
				}
				else
				{
					user->buffer += c;
				}
			}

		}
	}
}

void Server::EndConn()
{
	closesocket(servSock);
	WSACleanup();
}

vector<string> Server::Split(const string& str, char Delimiter) {
	istringstream ss(str);
	string buffer;
	vector<string> words;

	while (getline(ss, buffer, Delimiter)) {
		words.push_back(buffer);
	}

	return words;
}
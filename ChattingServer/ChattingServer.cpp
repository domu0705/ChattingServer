// -----------------------------------------------------------------------------------
//  서버와 클라이언트의 연결 및 입출력을 관리
// -----------------------------------------------------------------------------------
#include "USER.h"
#include "Manager.h"
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#define BUF_SIZE 30

using namespace std;

string charVecToStr(vector<char> &c) //안씀
{
	string str = "";
	for (int i = 0;i < c.size();i++)
	{
		cout << "c[" << i << "]" << "= " << c[i] << endl;
		str += c[i];
	}
	return str;
}

vector<string> split(string str, char Delimiter) {
	istringstream ss(str);             // istringstream에 str을 담기
	string buffer;                      // 구분자를 기준으로 절삭된 문자열이 담겨지는 버퍼

	vector<string> result;

	// istringstream은 istream을 상속받으므로 getline을 사용할 수 있음
	while (getline(ss, buffer, Delimiter)) {
		result.push_back(buffer);               // 절삭된 문자열을 vector에 저장
	}

	return result;
}

int main()
{
	Manager manager;
	WSADATA	wsaData;//WSADATA 구조체에는 Windows 소켓 구현에 대한 정보가 포함되어 있음
	SOCKET servSock, clntSock;
	SOCKADDR_IN servAddr, clntAddr;//68p

	u_short portNum = 2222; // win header에서 unsigned short 를 define한 것이 u_short 


	//MAKEWORD(2, 2):사용할 소켓의 버전이 2.2 (즉, 주버전 2,부버전2) 로 0x0202를 전달해야함. makeword는 2,2를 바이트 단위로 쪼개서 0x0202를 반환해줌
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //소켓 라이브러리 초기화
		cout << "WSAStartup() error!";

	servSock = socket(PF_INET, SOCK_STREAM, 0);//소켓 생성 (PF_INET= 소켓이 사용할 프로토콜 체계정보 전달(PF_INET는 1pv4라는 뜻), SOCK_STREAM=소켓의 데이터 전달 방식, 0=두컴퓨터간 통신에 사용되는 프로토콜 정보 전달)51p
	if (servSock == INVALID_SOCKET)
		cout << "socket() error";

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET; // AF_INET은 ipv4에 적용하는 주소체계
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);//73p,82p
	servAddr.sin_port = htons(portNum);//(host to network short.)short형 데이터를 네트워크 바이트 순서에서 호스트 바이트 순서로 변환

	//bind함수의 두번째 인자로 port,ip주소 정보를 담는 sockaddr구조체 변수의 주소값을 요구
	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)//소켓에 IP주소와 PORT번호 할당
		cout << "bind() error" << endl;
	else
		cout << "bind() success" << endl;

	if (listen(servSock, 5) == SOCKET_ERROR) //listen()은 성공시 0(숫자 0), 실패시 SOCKET_ERROR 를 반환. listen()을 통해 hServSock 을 서버소켓으로 완성.클라의 연결 요청을 받을 수 있는 상태가 됨
		cout << "listen() error";
	else
		cout << "listen() success" << endl;



	//SELECT 사용환경 설정
	TIMEVAL timeout;
	fd_set reads, cpyReads;//파일 디스크립터 배열(0,1로 표현되는 bit단위)

	int adrSz;
	int strLen, result;

	FD_ZERO(&reads);//인자로 전달된 주소의 fd_set형 변수의 모든 비트를 0으로 초기화 함
	FD_SET(servSock, &reads); //&reads주소의 변수에 1번 인자로 전달된 파일 디스크립터 정보를 등록함

	string buf;// 클라이언트의 문자열을 받아 저장할 string생성 (while문 안에서 생성하면 클라가 입력한 값들이 다 timeout돌면서 초기화돼서 사라짐)
	char c;

	while (1)
	{
		cpyReads = reads;//select함수 호출이 끝나면 변화가 생긴 파일디스크립터 위치를 제외한 나머지는 다 0으로 초기화 돼서 원본 유지를 위해 복사해줌.
		timeout.tv_sec = 5;//select 에서 무한정 블로킹 상태를 막기 위한 타임아웃(sec)
		timeout.tv_usec = 0;//select 에서 무한정 블로킹 상태를 막기 위한 타임아웃(micro sec)

		if ((result = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
		{
			cout << "SOCKET error" << endl;
			break;
		}

		if (result == 0) 
		{
			cout << "no change in fd set" << endl;
			continue;
		}


		for (int i = 0; i < int(reads.fd_count); i++)//select가 1 이상 반환됐을 때 실행됨
		{
			if (FD_ISSET(reads.fd_array[i], &cpyReads)) // FD_ISSET로 상태변화가 있었던(수신된 데이터가 있는 소켓의)파일 디스크립터를 찾음
			{
				cout << "상태변화 있음" << endl;

				if (reads.fd_array[i] == servSock) // 서버 소켓에서 변화가 있었는지 확인. 맞다면 연결 요청을 수락하는 과정 진행.(connection request)
				{
					adrSz = sizeof(clntAddr);
					clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &adrSz);
					User user(clntSock);//유저와 소켓 연결 정보 저장
					manager.SetUserToUserSockAry(clntSock, user);

					FD_SET(clntSock, &reads);//reads는 FD_set 배열임. 이 배열의 clntSock 인덱스가0 에서 1 로 바뀔듯

					cout << "client  연결 성공. clntSock: " << clntSock << endl;
					string msg = "* * 안녕하세요.텍스트 채팅 서버 ver 0.1입니다.\n\r* * 로그인 명령어(LOGIN)를 사용해주세요 !\n\r "; //\r 은 현재 줄의 맨 앞으로 커서를 옮겨줌. 다음 줄의 맨 앞부터 쓰고 싶다면 \n하고 \r하는 것 추천
					send(clntSock, msg.c_str(), int(msg.size()), 0);    // c_str 는 string을 char * 로 변환
				}
				else    // 상태가 변한 소켓이 서버소켓이 아님.  즉 수신할 데이터가 있음. (read message)
				{		//단 이경우에도 수신한 데이터가 문자열 데이터인지 or 연결종료를 의미하는 EOF인지 확인해야 함
					strLen = recv(reads.fd_array[i], &c, sizeof(char), 0);

					if (strLen == 0)    // close request!
					{
						FD_CLR(reads.fd_array[i], &reads);
						closesocket(cpyReads.fd_array[i]);
						cout << "closed client:" << cpyReads.fd_array[i] << endl;
					}
					else if (c == '\n') // 클라가 엔터를 입력했다면 여기로 가서 답을 줘야할 듯
					{

						string strBuf = buf.substr(0, buf.length() - 1);// 뒤에 자동으로 오는 \r을 제거
						vector<string> message = split(strBuf, ' ');
						cout << "strBuf 는" << strBuf << "입니다."<<endl;

						//cout << "user.GetState() = " << manager.userAry[reads.fd_array[i]].GetState() << endl;
						if (manager.userAry[reads.fd_array[i]].GetState() == State::WAITING) // LOGIN 이전의 상태. 로그인해야함
						{
							if (message.size() > 1 && message[0] == "LOGIN" && message[1].length() > 0)
							{
								//@@여기서 유저의 ID를 저장해야하는데 소켓번호밖에 모름. 소켓번호로 유저를 찾아야 함
								//user.SetID(message[1])
								/*string msg = "\r\n----------------------------------------------\n\r 반갑습니다. 텍스트 채팅 서버 ver 0.1 입니다.\n\r 이용중 불편하신 점이 있으면 아래 이메일로 문의 바랍니다.\n\r 감사합니다.\n\r programmed & arranged by Minjee Kim\n\r email: minjee.kim@nm-neo.com\n\r----------------------------------------------\n\r명령어안내(H) 종료(X)\n\r";
								send(reads.fd_array[i], msg.c_str(), int(msg.size()), 0);
								cout << "User ID : " << message[1] << endl;
								manager.userAry[reads.fd_array[i]].SetID(message[1]);
								manager.userAry[reads.fd_array[i]].SetState(State::LOBBY);*/
								manager.LogIn(reads.fd_array[i], message[1]);
							}
							else //로그인 실패
							{
								if (message.size() > 1) {
									cout << "message.size()=" << message.size() << " message[0]=" << message[0] << " message[1] = " << message[1] << endl;
								}
								else
									cout << "message.size()=" << message.size() << " message[0]=" << message[0] << endl;
								string msg = "** 올바른 사용법은 LOGIN [ID] 입니다.";
								send(reads.fd_array[i], msg.c_str(), int(msg.size()), 0);
							}

						}
						else if (manager.userAry[reads.fd_array[i]].GetState() == State::LOBBY)
						{
							cout << "새 명령은" << message[0] <<"입니다" <<endl;
							if (message[0].length() == 1 && message[0] == string("H"))
							{
								manager.H(reads.fd_array[i]);
							}
							else if (message[0].compare("US") == 0) // compare() : 같으면 0 , 다르면 -1 리턴
							{
								manager.US(reads.fd_array[i]);
							}
							else if (message[0].compare("LT") == 0)
							{
								manager.LT(reads.fd_array[i]);
							}
							else if (message[0].compare("ST") == 0)
							{
								manager.ST(reads.fd_array[i]);
							}
							else if (message[0].compare("PF") == 0)
							{
								manager.PF(reads.fd_array[i]);
							}
							else if (message[0].compare("TO") == 0)
							{
								manager.TO(reads.fd_array[i]);
							}
							else if (message[0].compare("O") == 0)
							{
								manager.O(reads.fd_array[i]);
							}
							else if (message[0].compare("J") == 0)
							{
								manager.J(reads.fd_array[i]);
							}
							else if (message[0].compare("X") == 0)
							{
								manager.X(reads.fd_array[i]);
							}
							else
							{
								manager.NotExistingCommend(reads.fd_array[i]);
							}
						}
						else{}
						buf.clear();


					}
					else // 클라가 문자를 입력했다면 (수신할 데이터가 문자열인 경우)
					{
						//지금은 여기서 문자 하나 받자마자 바로 답함 이걸 고쳐야 함
						//const char c[] = "LOG IN [사용자 이름] 으로 로그인 해주세요\n";
						//send(reads.fd_array[i], c, int(strlen(c)), 0);    // echo!
						buf+=c;
					}
				}
			}
		}
	}
	closesocket(servSock);
	WSACleanup();
	return 0;
}
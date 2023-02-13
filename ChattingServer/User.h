// -----------------------------------------------------------------------------------
//  사용자의 상태 관리
// -----------------------------------------------------------------------------------

#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>

using namespace std;

class USER
{
private:
	SOCKET socketNum;
	bool isLogIn;
	string ID;

public:
	USER(SOCKET socketNum);

	bool GetIsLogIn();
	string GetID();
	void SetID(string ID);

};

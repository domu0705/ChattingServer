#include "Server.h"


int main()
{
	Server server;

	server.PrepareServer();
	server.StartConn();
	server.EndConn();

	return 0;
}


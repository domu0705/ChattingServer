#pragma once

#include <string>
#include <vector>
using namespace std;

enum Protocol
{
	Long_Bar,
	Sentance_US,
	Sentance_LT,
	Sentance_ST,
	Sentance_PF,
	Sentance_TO,
	Sentance_O,
	Sentance_J,
	Sentance_X

};

class StringData
{
private:
public:
	vector<Protocol, string> key_vec;

	StringData();

	void GenerateData();

};

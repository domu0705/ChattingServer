#pragma once

#include <string>
#include <map>


enum Sentance
{
	Sentance_H,
	Sentance_US,
	Sentance_LT,
	Sentance_ST,
	Sentance_PF,
	Sentance_TO,
	Sentance_O,
	Sentance_J,
	Sentance_X

};

class Data
{
private:
public:
	//map<Sentance, std::string> dataAry;//socket과 client 묶을 자료형

	Data();

	void GenerateData();

};

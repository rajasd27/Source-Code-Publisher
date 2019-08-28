////////////////////////////////////////////////////////////////////////////////////////////////////
// parserTable.cpp - Provides functionality to an unordered map to store the parsing information  //
// ver 1.0                                                                                        //
// Language:    C++, Visual Studio 2017                                                           //
// Application: Spring 2019 Project 2, CSE687 - Object Oriented Design                            //
// Author:      Rajas Deshpande, Syracuse University                                              //
//              rdeshpan@syr.edu                                                                  //
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <unordered_map>
#include <string>
#include "parserTable.h"

std::unordered_map<int, parserTable::depPair> parserTable::parseTable_;

parserTable::parserTable()
{
}


parserTable::~parserTable()
{
}

void parserTable::addDependency(int key, depPair pair)
{
	parseTable_.insert({ key, pair });
}

parserTable::depPair parserTable::getValue(int key)
{
	auto search = parseTable_.find(key);
	if (search != parseTable_.end()) {
		return search->second;
	}
	else {
		return depPair();
	}
}

void parserTable::displayParserTable()
{
	std::cout << "\n\n -------------------- PARSER TABLE -------------------- \n";
	for (auto data : parseTable_) {
		std::cout << "\n + " << data.first << "\n";
		std::cout << "\t- " << data.second.first << "\n";
		std::cout << "\t- " << data.second.second << "\n";
}
}

#ifdef TEST_PARSER

#include "parserTable.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

	parserTable pt;
	pt.clearTable();
	std::pair<int, std::string> parsingInfo;
	auto temp = std::make_pair("function",50);
	pt.addDependency(10, temp);
	auto temp = std::make_pair("class", 8);
	pt.addDependency(1, temp);

	pt.displayParserTable();

	auto size = pt.getSize();
	std::cout << "\nSize of Parser table: " << size;
	
	auto pair = pt.getValue(10);
	auto first = pair.first;
	auto second = pair.second;
	std::cout << "\nPair retrieved from the table: " << first << "\t" << second;

	return 0;
}

#endif // TEST_PARSER
#pragma once
/////////////////////////////////////////////////////////////////////////////////////////////
// parserTable.h - Provides functionality to carry out operations on an unordered map      //
// ver 1.0                                                                                 //
// Language:    C++, Visual Studio 2017                                                    //
// Application: Spring 2019 Project 2, CSE687 - Object Oriented Design                     //
// Author:      Rajas Deshpande, Syracuse University                                       //
//              rdeshpan@syr.edu                                                           //
/////////////////////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* The parserTable Package consists of a parserTable class that provides functionality
* to manipulate an unordered map. An unordered map is used as a parser table that stores
* the parsing information provided by the parser
*
* This package provides functions:
* - clearTable()                            deletes all unordered map content
* - addDependency(int, std::pair)           adds key and value into the unordered map
* - getSize()                               returns size of the unordered map
* - getValue(int)                           returns value corresponding to the given key
* - displayParserTable()                    displays the unordered map
*
* Build Process:
* --------------
* Required Files:
*   parserTable.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 04 March 2019
* - first release
*
* Notes:
* ------
* - Designed to provide all functionality in header file.
* - Implementation file only needed for test and demo.
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/

#include <iostream>
#include <unordered_map>
#include <string>


class parserTable
{
public:

	using depPair = std::pair<std::string, int>;

	parserTable();
	~parserTable();

	void clearTable() { parseTable_.clear(); }	
	void addDependency(int, depPair);
	int getSize() { return parseTable_.size(); }
	depPair getValue(int);
	void displayParserTable();

private:
	static std::unordered_map<int, depPair> parseTable_;
};


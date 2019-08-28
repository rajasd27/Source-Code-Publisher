#pragma once
/////////////////////////////////////////////////////////////////////////////////////////////
// dependencyTable.h - Provides functionality to carry out operations on an unordered map  //
// ver 1.0                                                                                 //
// Language:    C++, Visual Studio 2017                                                    //
// Application: Spring 2019 Project 2, CSE687 - Object Oriented Design                     //
// Author:      Rajas Deshpande, Syracuse University                                       //
//              rdeshpan@syr.edu                                                           //
/////////////////////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* The dependencyTable Package consists of a dependencyTable class that provides functionality
* to manipulate an unordered map. An unordered map is used as a dependency table that stores 
* file names as keys and their dependencies as values
*
* This package provides functions:
* - findDependency(string, vector)	   finds all dependency names matching a regular expression
* - addDependency(string, vector)      adds key value to the unordered map
* - display()			               displays the unordered map
* - clearTable()			           deletes all entries in the unordered map
* - getFiles(fileName)                 returns values matching a key which passed as an argument
* - checkKey(fileName)                 checks whether a key is present in the map
*
* Build Process:
* --------------
* Required Files:
*   dependencyTable.h
*	FileSystem.h
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

class dependencyTable
{
public:

	using fileName = std::string;
	using fileDeps = std::vector<std::string>;

	fileDeps findDependency(fileName, fileDeps);
	void addDependency(fileName, fileDeps);
	void display();
	void clearTable() { depTable_.clear(); }
	fileDeps getFiles(fileName);
	bool checkKey(fileName);

	dependencyTable();
	~dependencyTable();

private:
	static std::unordered_map<fileName, fileDeps> depTable_;
};


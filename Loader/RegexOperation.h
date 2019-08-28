//////////////////////////////////////////////////////////////////////////////////////
// RegexOperation.h - Provides functionality to extract relevent data, matching     //
//                    a given regular expression                                    //
// ver 1.0                                                                          //
// Language:    C++, Visual Studio 2017                                             //
// Application: Spring 2019 Project 1, CSE687 - Object Oriented Design              //
// Author:      Rajas Deshpande, Syracuse University                                //
//              rdeshpan@syr.edu                                                    //
//////////////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* The RegexOperation Package consists of a RegexOperation class that takes a regular expression
* as input from the command line and extracts all the files that match the regular expression.
*
* This package provides functions:
* - isValidPattern(int, char*)	 checks if the given pattern is valid or not
* - fileProcessing(int, char*)   stores all files matching the regular expression in a
*    							 vector and returns that vector
* - applyRegex(vector)			 matches regular expression with the file vector
*
* Build Process:
* --------------
* Required Files:
*   RegexOperation.h
*	DirExplorerN.h
*	Utilities.h
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


#pragma once

#include <iostream>
#include <vector>

class RegexOperation 
{
public:

	using filesVect = std::vector<std::string>;

	RegexOperation();
	~RegexOperation();

	int isValidPattern(int argc, char* argv[]);
	filesVect fileProcessing(int argc, char* argv[]);
	filesVect applyRegex(filesVect, int argc, char* argv[]);

private:
	filesVect finalVector_;
};

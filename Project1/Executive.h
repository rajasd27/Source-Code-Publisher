	#pragma once
//////////////////////////////////////////////////////////////////////////
// Executive.h - Header File for Executive                              //
// ver 1.0                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: Spring 2019 Project 1, CSE687 - Object Oriented Design  //
// Author:      Rajas Deshpande, Syracuse University                    //
//              rdeshpan@syr.edu                                        //
//////////////////////////////////////////////////////////////////////////

/*
*
* Build Process:
* --------------
* Required Files:
*   Executive.h
*
* Maintenance History:
* --------------------
* ver 1.1 : 08 April 2019
* - Added a new function executePublisher
*
* ver 1.0 : 03 Feb 2019
* - first release
*
* Notes:
* ------
* - none yet
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/

#include <vector>

class Executive
{
public:
	Executive();
	~Executive();

	using stringVector = std::vector<std::string>;

	std::vector<std::string> executePublisher(int argc, char* argv[]);
	stringVector getDependencies(std::vector<std::string>, std::vector<std::string>);
	void setDepVect(stringVector);
	stringVector getDepVect();

private:
	stringVector depVect_;
};

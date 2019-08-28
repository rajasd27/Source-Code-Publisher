//////////////////////////////////////////////////////////////////////////////////////
// Dependency.h - Parses the input file and finds all dependency links            //
// ver 1.0                                                                          //
// Language:    C++, Visual Studio 2017                                             //
// Application: Spring 2019 Project 2, CSE687 - Object Oriented Design              //
// Author:      Rajas Deshpande, Syracuse University                                //
//              rdeshpan@syr.edu                                                    //
//////////////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* The Dependency Package consists of a Dependency class that finds all the dependency 
* files and returns them to the converter class to convert them into valid HTML format.
* It also provides functionality to parse the given input file to find different sections
* of the file like class, functions and namespaces.
*
* This package provides functions:
* - extractIncludes(vector)	                extracts all preprocessor definitions
* - checkRVector(vector, vector)            checks whether the dependent files are present in the vector
*                                           returned by the loader package
* - executeParser(string)                   matches regular expression with the file vector
* - myTreeWalk(CodeAnalysis::ASTNode*)      recursive function to find all sections of the code

* Build Process:
* --------------
* Required Files:
*   Dependency.h
*   Parser.h
*   Utilities.h
*   Toker.h
*   Semi.h
*   ActionsAndRules.h
*   ConfigureParser.h
*   FileSystem.h
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

#include <vector>
#include "../Utilities/Utilities.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"

#define Util Utilities::StringHelper

class Dependency
{
public:
	using depVector = std::vector<std::string>;

	Dependency();
	~Dependency();

	depVector extractIncludes(depVector);
	depVector checkRVector(depVector, depVector);
	void executeParser(std::string);
	void myTreeWalk(CodeAnalysis::ASTNode*);


private:
	depVector includes_;
	depVector finalV_;
};


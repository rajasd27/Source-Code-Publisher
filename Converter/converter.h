#pragma once
//////////////////////////////////////////////////////////////////////////////////
// converter.h - Converts files matching a regular expression into HTML script  //
// ver 1.1                                                                      //
// Language:    C++, Visual Studio 2017                                         //
// Application: Spring 2019 Project 2, CSE687 - Object Oriented Design          //
// Author:      Rajas Deshpande, Syracuse University                            //
//              rdeshpan@syr.edu                                                //
//////////////////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* The Converter Package consists of a converter class that converts a vector of files, 
* into HTML script of the respective files. It adds division tags and buttons that gives
* the user a flexibility to hide and show comments, classes, functions and namespaces.
* The converted HTML scripts are stored into a directory named ConvertedPages, created 
* under the same project directory.
* 
* This package provides functions: 
* - createDirectory()			       creates a new directory under the project directory
* - loadHead()                         loads HTML head tag from a template file
* - addHref(string)                    adds a reference tag to the HTML file
* - addReturnHref(string)              adds a reference tag to the previous page 
* - loadBody()                         loads HTML body tag from a template file
* - loadEnd()                          adds end tags to close the HTML head and body tags
* - addPreTag()                        adds a pre tag to the HTML file
* - endPreTag()                        add an end pre tag to the HTML file
* - addDivTagsToSComments(string)      adds HTML division tags to single-line comments
* - addDivTagsToMComments(string)      adds HTML division tags to multi-line comments
* - addDivTags(string, int)            adds HTML division tags to classes, functions and namespaces
* - addButton(string)                  adds HTML button 
* - findMarkup(string)                 replaces all markup characters
* - createHtmlFiles(vector)            converts file text to HTML script
*
* Build Process:
* --------------
* Required Files:
*   converter.h
*	FileSystem.h
*	Dependency.h
*	dependencyTable.h
*
* Maintenance History:
* --------------------
* ver 1.1 : 04 March 2019
* - removed pattern matching functionality from the converter class
* - added functions to read html head and html body tags from a template file
* - added functions to handle html div tags and buttons.
* ver 1.0 : 03 Feb 2019
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
#include <vector>
#include <string>

class converter
{
public:

	using convertFiles = std::vector<std::string>;

	converter();
	~converter();

	void createDirectory();
	std::string loadHead();
	std::string addHref(std::string);
	std::string addReturnHref(std::string);
	std::string loadBody();
	std::string loadEnd();
	std::string addPreTag();
	std::string endPreTag();
	std::string addDivTagsToSComments(std::string);
	std::string addDivTagsToMComments(std::string);
	std::string addDivTags(std::string, int);
	std::string addButton(std::string);
	std::string findMarkup(std::string);
	convertFiles createHtmlFiles(convertFiles);

private:
	std::string directory_;
	std::string storedirectory_;
	std::string htmlScript_;
	convertFiles htmlVector_;
	std::string comments_;
	std::vector <int> end_;
};



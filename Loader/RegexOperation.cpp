//////////////////////////////////////////////////////////////////////////////////////
// RegexOperation.cpp - Provides functionality to extract relevent data matching    //
//						a regular expression                                        //
// ver 1.0                                                                          //
// Language:    C++, Visual Studio 2017                                             //
// Application: Spring 2019 Project 1, CSE687 - Object Oriented Design              //
// Author:      Rajas Deshpande, Syracuse University                                //
//              rdeshpan@syr.edu                                                    //
//////////////////////////////////////////////////////////////////////////////////////

#include "RegexOperation.h"
#include <iostream>
#include <vector>
#include <regex>
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Utilities/Utilities.h"

RegexOperation::RegexOperation()
{
}


RegexOperation::~RegexOperation()
{
}

int RegexOperation::isValidPattern(int argc, char* argv[])
{
	Utilities::ProcessCmdLine pcl(argc, argv);

	Utilities::preface("Command Line: ");
	pcl.showCmdLine();
	Utilities::putline();

	if (pcl.parseError())
	{
		pcl.usage();
		std::cout << "\n\n Unable to Parse the given Pattern. Exiting.................\n\n";
		return 1;
	}

	return 0;
}

RegexOperation::filesVect RegexOperation::fileProcessing(int argc, char* argv[])
{
	Utilities::ProcessCmdLine pcl(argc, argv);
	FileSystem::DirExplorerN de(pcl.path());

	for (auto patt : pcl.patterns())
	{
		de.addPattern(patt);
	}

	if (pcl.hasOption('s'))
	{
		de.recurse();
	}

	de.search();

	std::cout << "\n\n--------- Total number of Files found ---------\n";

	de.showStats();

	std::cout << "\n\n--------- Files retrieved after parsing the pattern ---------\n\n";

	for (auto files : de.getVector())
	{
		std::cout << files << "\n";
	}

	return de.getVector();
}

RegexOperation::filesVect RegexOperation::applyRegex(filesVect pattVector, int argc, char * argv[])
{
	Utilities::ProcessCmdLine pcl(argc, argv);
	FileSystem::DirExplorerN de(pcl.path());

	if (pcl.regexes().size() == 0) {
		std::cout << "\n\n ----------------------- Regex Not Found ----------------------- \n\n";
		return pattVector;
	}

	std::cout << "\n\n --------------------- Files accepted after applying Regex --------------------- \n\n";

	for (auto re : pcl.regexes()) {
		std::cout << "Regular Expression : " << re << "\n\n";
		std::regex r(re);
		for (auto files : pattVector) {
			std::string tempStr = FileSystem::Path::getName(files);
			if (std::regex_match(tempStr, r)) {
				std::cout << files << " matched to regex " << re << "\n";
				finalVector_.push_back(files);
			}
		}
		std::cout << "\n";
	}

	return finalVector_;
}

#ifdef TEST_REGEX

#include "RegexOperation.h"
#include <iostream>

int main(int argc, char* argv[]) {

	RegexOperation robj;

	auto var = robj.isValidPattern(argc, argv);
	if (var == 1) {
		return 1;
	}

	auto filesVect = robj.fileProcessing(argc, argv);
	filesVect = robj.applyRegex(filesVect, argc, argv);

	return 0;
}

#endif


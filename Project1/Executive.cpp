//////////////////////////////////////////////////////////////////////////
// Executive.cpp - Executive Main File                                  //
// ver 1.2                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: Spring 2019 Project 2, CSE687 - Object Oriented Design  //
// Author:      Rajas Deshpande, Syracuse University                    //
//              rdeshpan@syr.edu                                        //
//////////////////////////////////////////////////////////////////////////


#include "Executive.h"
#include <vector>
#include <iostream>
#include "../Converter/converter.h"
#include "../Loader/RegexOperation.h"
#include "../Dependency/Dependency.h"

Executive::Executive()
{
}

Executive::~Executive()
{
}

void Executive::setDepVect(stringVector fv) {
	depVect_ = fv;
}

Executive::stringVector Executive::getDepVect() {
	return depVect_;
}

std::vector<std::string> Executive::executePublisher(int argc, char* argv[]) {
	std::cout << "\n\nREQUIREMENT 1 --------- This project uses Visual Studio 2017 and its C++ Windows Console Projects ---------\n\n";
	std::vector<std::string> returnVect;
	RegexOperation regObj;
	auto returnValue = regObj.isValidPattern(argc, argv);
	
	if (returnValue == 1) {
		std::cout << "\n\n---------------------------- Given Pattern is not valid ----------------------------\n\n";
		return {};
	}
	
	auto fileVect = regObj.fileProcessing(argc, argv);
	if (fileVect.size() == 0) {
		std::cout << "\n\n--------- No Files matched to the given Pattern ---------\n\n";
		return {};
	}

	fileVect = regObj.applyRegex(fileVect, argc, argv);
	
	if (fileVect.size() == 0) {
		std::cout << "\n\n--------- No Files matched to the given Regex ---------\n\n";
		return {};
	}

	auto fileVect1 = fileVect;

	Dependency depObj;
	fileVect = depObj.extractIncludes(fileVect);

	converter convObj;
	fileVect = convObj.createHtmlFiles(fileVect);

	auto fv = getDependencies(fileVect1, fileVect);
	setDepVect(fv);

	return fileVect1; 
}

Executive::stringVector Executive::getDependencies(stringVector convFiles, stringVector depFiles) {
	stringVector temp;
	for (auto d : depFiles) {
		for (auto c : convFiles) {
			if (FileSystem::Path::getName(c) + ".html" != FileSystem::Path::getName(FileSystem::Path::getFullFileSpec(d))) {
				temp.push_back(d);
			}
		}
	}	
	return temp;
}

#ifdef TESTEXE

int main(int argc, char* argv[]) {

	std::cout << "\n\nREQUIREMENT 1 ---------This project uses Visual Studio 2017 and its C++ Windows Console Projects ---------\n\n";

	try {
		std::cout << "\n\nREQUIREMENT 3 --------- Entered in Executive Package ---------\n\n";

		RegexOperation regObj;
		Dependency depObj;
		converter convObj;
		Display dispObj;

		auto returnValue = regObj.isValidPattern(argc, argv);

		if (returnValue == 1) {
			std::cout << "\n\n---------------------------- Given Pattern is not valid ----------------------------\n\n";
			return 1;
		}

		auto fileVect = regObj.fileProcessing(argc, argv);

		if (fileVect.size() == 0) {
			std::cout << "\n\n--------- No Files matched to the given Pattern ---------\n\n";
			return 1;
		}

		fileVect = regObj.applyRegex(fileVect, argc, argv);
		auto fileVect1 = fileVect;

		fileVect = depObj.extractIncludes(fileVect);
		fileVect = convObj.createHtmlFiles(fileVect);
		dispObj.displayHtmlFiles(fileVect1);
		std::cout << "\n\n";

		return 0;
	}

	catch (const std::exception& e) {
		std::cout << "\n\nProgram exited with exception: " << e.what();
	}

	catch (...) {
		std::cout << "\n\nUnknown Exception....";
	}
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////////
// dependencyTable.cpp - Provides functionality to carry out operations on an unordered map //
// ver 1.0                                                                                  //
// Language:    C++, Visual Studio 2017                                                     //
// Application: Spring 2019 Project 2, CSE687 - Object Oriented Design                      //
// Author:      Rajas Deshpande, Syracuse University                                        //
//              rdeshpan@syr.edu                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////

#include "dependencyTable.h"
#include <regex>
#include "../FileSystem/FileSystem.h"

using namespace FileSystem;
std::unordered_map<dependencyTable::fileName, dependencyTable::fileDeps> dependencyTable::depTable_;

dependencyTable::dependencyTable()
{
}


dependencyTable::~dependencyTable()
{
}

dependencyTable::fileDeps dependencyTable::findDependency(fileName f, fileDeps inc)
{
	std::string re = "(^\\s*#\\s*include\\s*\")(.*)(\"\\s*$)";
	std::regex r(re);
	std::smatch group;
	fileDeps depF;
	depF.clear();

	for (auto dep : inc) {
		if (std::regex_match(dep, group, r)) {
			std::string path = group[2];
			//path = FileSystem::Path::getPath(f) + "//" + path;
			path = FileSystem::Path::getPath(f) + path;
			if (FileSystem::File::exists(path)) {
				path = FileSystem::Path::getFullFileSpec(path);
				depF.push_back(path);
			}
			else {
				std::cout << "\n   -> Dependency File: " << path << " not found!" << std::endl;
			}
		}
	}

	return depF;
}

void dependencyTable::addDependency(fileName fname, fileDeps fdep)
{
	fname = FileSystem::Path::getName(fname);
	auto search = depTable_.find(fname);
	if (search != depTable_.end()) {
		std::cout << "\n   -> Dependencies for file " << fname << " present in the Dependency Table.....\n";
	}
	else {
		depTable_.insert({ fname, fdep });
		std::cout << "\n   -> Dependencies for file " << fname << " inserted in the Dependency Table.....\n";
	}
}

void dependencyTable::display()
{
	std::cout << "\n\n -------------------- DEPENDENCY TABLE -------------------- \n";
	std::cout << "\n ** The dependency table displays dependency links of all the files matching the     **";
	std::cout << "\n ** regular expression and also the dependency links of the dependent files as well. **\n";
	for (auto data : depTable_) {
		std::cout << "\n + " << data.first << "\n";
		for (auto dep : data.second) {
			std::cout << "\t- " << dep << "\n";
		}
	}
}

dependencyTable::fileDeps dependencyTable::getFiles(fileName key)
{
	auto search = depTable_.find(key);
	if (search != depTable_.end()) {
		return search->second;
	}
	else {
		std::cout << "\nKey " << key << " not found." << std::endl;
		return fileDeps();
	}
}

bool dependencyTable::checkKey(fileName fname)
{
	fname = FileSystem::Path::getName(fname);
	auto search = depTable_.find(fname);
	if (search != depTable_.end()) {
		return true;
	}
	else
		return false;
}


#ifdef TEST_DEPENDENCIES

#include "dependencyTable.h"
#include <iostream>

int main(int argc, char* argv[]) {

	dependencyTable dt;
	dt.clearTable();
	std::vector<std::string> includes;
	includes.push_back("#include <iostream>");
	includes.push_back("#include <vector>");
	includes.push_back("#include \"../Converter/converter.h\"");
	includes.push_back("#include \"../Converter/converter.cpp\"");

	std::string file = "dependencyTable.h";
	auto dependencies = dt.findDependency(file, includes);
	dt.addDependency(file, dependencies);
	dt.display();

	auto depend = dt.getFiles(file);
	if (checkKey(fileName)) {
		std::cout << "\nKey Found";
	}
	else {
		std::cout << "\nKey Not Found";
	}

	return 0;
}

#endif // TEST_DEPENDENCIES
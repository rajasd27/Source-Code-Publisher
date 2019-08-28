//////////////////////////////////////////////////////////////////////////////////////
// Dependency.cpp - Parses the input file and finds all dependency links            //
// ver 1.0                                                                          //
// Language:    C++, Visual Studio 2017                                             //
// Application: Spring 2019 Project 2, CSE687 - Object Oriented Design              //
// Author:      Rajas Deshpande, Syracuse University                                //
//              rdeshpan@syr.edu                                                    //
//////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <iostream>
#include "Dependency.h"
#include "../DependencyTable/dependencyTable.h"
#include "../DependencyTable/parserTable.h"
#include "../Parser/Parser.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../FileSystem/FileSystem.h"


Dependency::Dependency()
{
}


Dependency::~Dependency()
{
}


Dependency::depVector Dependency::extractIncludes(depVector files)
{
	dependencyTable depObj;
	depObj.clearTable();
	finalV_ = files;
	while (!files.empty())
	{
		std::string f = files.back();
		files.pop_back();
		includes_.clear();
		if (depObj.checkKey(f))
			continue;

		std::string fileSpec = FileSystem::Path::getFullFileSpec(f);
		std::string msg = "Processing file " + fileSpec;
		Util::title(msg);
		CodeAnalysis::ConfigParseForCodeAnal configure;
		CodeAnalysis::Parser* pParser = configure.Build();
		configure.Attach(fileSpec);
		std::string name = FileSystem::Path::getName(f);
		CodeAnalysis::Repository* pRepo = CodeAnalysis::Repository::getInstance();
		pRepo->package() = name;
		while (pParser->next())
			pParser->parse();

		std::cout << "\n";
		CodeAnalysis::ASTNode* pGlobalScope = pRepo->getGlobalScope();
		complexityEval(pGlobalScope);
		auto iter2 = pGlobalScope->statements_.begin();
		while (iter2 != pGlobalScope->statements_.end()) {
			includes_.push_back((*iter2)->ToString());
			++iter2;
		}
		auto tempVector = depObj.findDependency(f, includes_);
		depVector dVector;
		dVector.clear();

		for (auto d : tempVector)
			dVector.push_back(FileSystem::Path::getName(d));

		depObj.addDependency(f, dVector);
		finalV_ = checkRVector(finalV_, tempVector);
		for (auto f : tempVector)
			files.push_back(f);
	}
	depObj.display();
	return finalV_;
}

Dependency::depVector Dependency::checkRVector(depVector files, depVector depV)
{
	int flag;
	for (auto d : depV) {
		flag = 0;
		for (auto f : files) {
			if (FileSystem::Path::getName(d) == FileSystem::Path::getName(f)) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			files.push_back(d);
		}
	}

	return files;
}

void Dependency::executeParser(std::string f)
{
	parserTable pObj;
	std::string fileSpec;
	std::string name;

	fileSpec = FileSystem::Path::getFullFileSpec(f);

	CodeAnalysis::ConfigParseForCodeAnal configure;
	CodeAnalysis::Parser* pParser = configure.Build();

	configure.Attach(fileSpec);
	name = FileSystem::Path::getName(f);
	CodeAnalysis::Repository* pRepo = CodeAnalysis::Repository::getInstance();
	pRepo->package() = name;

	while (pParser->next())
		pParser->parse();

	CodeAnalysis::ASTNode* pGlobalScope = pRepo->getGlobalScope();
	complexityEval(pGlobalScope);

	pObj.clearTable();
	myTreeWalk(pGlobalScope);
}

void Dependency::myTreeWalk(CodeAnalysis::ASTNode * pGlobalScope)
{
	parserTable pObj;
	static size_t indentLevel = 0;
	auto iter = pGlobalScope->children_.begin();
	++indentLevel;

	auto temp = std::make_pair(pGlobalScope->type_, pGlobalScope->endLineCount_);
	pObj.addDependency(pGlobalScope->startLineCount_, temp);

	while (iter != pGlobalScope->children_.end())
	{
		myTreeWalk(*iter);
		++iter;
	}

	--indentLevel;
}



#ifdef TESTDEP

int main(int argc, char* argv[]) {

	Dependency dobj;
	dobj.clearTable();
	std::vector <std::string> depVector, mainVector;

	depVector.push_back("../Dependency/Dependency.h");
	mainVector.push_back("../../Dependency/Dependency.cpp");

	depVector = dobj.extractIncludes(depVector);
	depVector = dobj.checkRVector(depVector, depVector);

	dobj.executeParser("Dependency.h");

	auto size = dobj.getSize();
	std::cout << "\nSize of Parser Table : " << size;

	dobj.displayParserTable();

	int key = 10;
	auto temp = dobj.getFiles(key);
	std::cout << "\nValue for key " << key << ": " << temp.first << "\t" << temp.second;
 		 
	return 0;
}

#endif
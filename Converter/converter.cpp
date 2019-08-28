//////////////////////////////////////////////////////////////////////////////////////
// converter.cpp - Converts files matching a regular expression into HTML script    //
// ver 1.1                                                                         //
// Language:    C++, Visual Studio 2017                                             //
// Application: Spring 2019 Project 2, CSE687 - Object Oriented Design              //
// Author:      Rajas Deshpande, Syracuse University                                //
//              rdeshpan@syr.edu                                                    //
//////////////////////////////////////////////////////////////////////////////////////

#include "converter.h"
#include <iostream>
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../FileSystem/FileSystem.h"
#include "../Dependency/Dependency.h"
#include "../DependencyTable/dependencyTable.h"
#include "../DependencyTable/parserTable.h"


converter::converter()
{
	directory_ = "../ConvertedPages";
	storedirectory_ = ".";
	comments_ = "comments";
}


converter::~converter()
{
}


void converter::createDirectory()
{
	if (!FileSystem::Directory::exists(directory_)) {
		FileSystem::Directory::create(directory_);
	}
}

std::string converter::loadHead()
{
	std::string head;
	std::ifstream inStartHtml("../Converter/Head.txt");
	for (std::string line; std::getline(inStartHtml, line);) {
		head += "\n";
		head += line;
	}
	inStartHtml.close();

	return head;
}

std::string converter::addHref(std::string files)
{
	dependencyTable dObj;
	std::string href;
	auto deps = dObj.getFiles(FileSystem::Path::getName(files));

	if (deps.size() == 0) {
		href = "\t<br><h3> Dependencies: No Dependencies\n";
	}
	else {
		href += "\t<br><h3> Dependencies: \n";
		for (auto d : deps) {
			href += "\t\t &emsp;<a href = \"" + storedirectory_ + "/" + d + ".html\">" + d + "</a>\n";
		}
	}

	href += "\t</h3>\n";
	return href;
}

std::string converter::addReturnHref(std::string file)
{
	return "\t\t<br><a class = \"back\" href = \"javascript:history.back()\">Go To Previous Page</a><br>\n";
}

std::string converter::loadBody()
{
	std::string body;
	std::ifstream inBodyHtml("../Converter/Body.txt");
	for (std::string line; std::getline(inBodyHtml, line);) {
		body += "\n";
		body += line;
	}
	inBodyHtml.close();

	return body;
}

std::string converter::loadEnd()
{
	std::string end;
	std::ifstream inEndHtml("../Converter/End.txt");
	for (std::string line; std::getline(inEndHtml, line);) {
		end += "\n";
		end += line;
	}
	inEndHtml.close();

	return end;
}

inline std::string converter::addPreTag()
{
	return "\n<pre>\n";
}

inline std::string converter::endPreTag()
{
	return "\n</pre>\n";
}

std::string converter::addDivTagsToSComments(std::string line)
{
	size_t offset = line.find('/');
	if (offset != std::string::npos) {
		if (line[offset + 1] == '/') {
			line.replace(offset, 0, "<div class = \"comments\">");
			line += "</div>";
			return line;
		}
	}

	return "";
}

std::string converter::addDivTagsToMComments(std::string line)
{
	size_t offset = line.find('/');
	size_t offset1 = line.find('*');
	size_t offsetr = line.rfind('*');

	if (offset != std::string::npos) {
		if (line[offset + 1] == '*' && line[offsetr + 1] == '/') {
			line.replace(offset, 0, "<div class = \"comments\">");
			line.replace(offset + 24, 0, "</div>");
		}

		else if (line[offset + 1] == '*') {
			line.replace(offset, 0, "<div class = \"comments\">");
		}

		else if (line[offset1 + 1] == '/') {
			line.replace(offset1 + 2, 0, "</div>");
		}

		return line;
	}

	return line;
}

std::string converter::addDivTags(std::string line, int key)
{
	parserTable pObj;
	auto pair = pObj.getValue(key);

	if (pair.first != "") {
		if (pair.first == "class" || pair.first == "namespace" || pair.first == "function") {
			if (key == pair.second) {
				return line;
			}

			std::string tagName = pair.first + std::to_string(key);
			std::string divStr = "<div class = \"" + tagName + "\">";
			end_.push_back(pair.second);
			std::string butTag = addButton(tagName);
			size_t offset = line.find('{');
			line.replace(offset, 0, divStr);
			line = butTag + line;
		}
	}

	else {
		for (auto e : end_) {
			if (key == e) {
				size_t offset = line.find('}');
				if (line[offset + 1] == ';') {
					line.replace(offset + 2, 0, "</div>");
				}
				else {
					line.replace(offset + 1, 0, "</div>");
				}
			}
		}
	}
	return line;
}

std::string converter::addButton(std::string btn)
{
	std::string buttonName = "button1";
	if (btn == "comments")
		return "<br><button type = \"button\" onclick = \"toggleVisibility('" + btn + "')\" id = \"" + buttonName + "\">Hide Comments</button><br>";
	else {
		return "<button type = \"button\" onclick = \"toggleVisibility('" + btn + "')\" id = \"" + btn + "\">-</button><br>";
	}
}

std::string converter::findMarkup(std::string line)
{
	size_t index1 = line.find('<');
	while (index1 != std::string::npos) {
		line.replace(index1, 1, "&lt");
		index1 = line.find('<', index1 + 1);
	}

	size_t index2 = line.find('>');
	while (index2 != std::string::npos) {
		line.replace(index2, 1, "&gt");
		index2 = line.find('>', index2 + 1);
	}

	return line;
}

converter::convertFiles converter::createHtmlFiles(convertFiles filesToConvert)
{
	Dependency depObj;
	std::string str = "<div class = \"comments\">";
	std::cout << "\n\n--------- Converting the file text to a valid html file ---------\n\n\t\nConverting....\n";
	createDirectory();

	for (auto files : filesToConvert) {
		std::cout << "\t.\n";
		int i = 0;
		end_.clear();

		depObj.executeParser(files);
		std::string fileName = FileSystem::Path::getName(files);
		auto name = fileName;
		fileName += ".html";

		std::ifstream inFileStream(files);
		std::ofstream outFileStream(directory_ + "\\" + fileName);

		outFileStream << loadHead();
		std::string header = "<h2>File Currently Opened: " + name;
		outFileStream << "\n<title>" + name + "</title>";
		std::string data = loadBody() + header + addHref(files) + addReturnHref(files) + addButton(comments_) + addPreTag();
		outFileStream << data;

		for (std::string line; std::getline(inFileStream, line);) {
			i++;
			line = findMarkup(line);
			auto line1 = addDivTagsToSComments(line);
			line = addDivTagsToMComments(line);
			line = addDivTags(line, i);

			if (!line1.empty())
				outFileStream << line1;
			else {
				line += "\n";
				outFileStream << line;
			}
		}
		outFileStream << endPreTag() + loadEnd();
		htmlVector_.push_back(directory_ + "\\" + fileName);
		inFileStream.close();
		outFileStream.close();
	}
	std::cout << "\nAll Files Converted!\n";
	return htmlVector_;
}


#ifdef TESTCONV

int main(int argc, char* argv[]) {

	std::cout << "\n\n REQUIREMENT 8 ----------- An automated test to demonstrate that all the functional requirements are met -----------\n\n";

	converter convTestObj;

	using testVector = std::vector<std::string>;

	convTestObj.createDirectory();

	testVector tempVector;
	tempVector.push_back("../Converter/converter.h");

	tempVector = convTestObj.createHtmlFiles(tempVector);

	return 0;
}


#endif // TESTCONV

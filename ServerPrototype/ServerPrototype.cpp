//////////////////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages               //
// ver 1.1                                                                          //
// Language:    C++, Visual Studio 2017                                             //
// Application: Spring 2019 Project 4, CSE687 - Object Oriented Design              //
// Author:      Rajas Deshpande, Syracuse University                                //
//              rdeshpan@syr.edu                                                    //
//////////////////////////////////////////////////////////////////////////////////////

#include "ServerPrototype.h"
#include "../FileSystem/FileSystem.h"
#include "../Project1/Executive.h"
#include "../Utilities/Utilities.h"
#include <chrono>

namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

//----< Function to get all files in a given path >------------------

Files Server::getFiles(const Repository::SearchPath& path)
{
  return Directory::getFiles(path);
}

//----< Function to get all directories in a given path >------------------

Dirs Server::getDirs(const Repository::SearchPath& path)
{
  return Directory::getDirectories(path);
}

//----< Displays message details on console >------------------

template<typename T>
void show(const T& t, const std::string& msg)
{
  std::cout << "\n  " << msg.c_str();
  for (auto item : t)
  {
    std::cout << "\n    " << item.c_str();
  }
}

//----< Procedure that executes when client sends a echo command >------------------

std::function<Msg(Msg)> echo = [](Msg msg) {
  Msg reply = msg;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command(msg.command());
  return reply;
};

//----< Procedure that executes when client sends a getFiles command >------------------

std::function<Msg(Msg)> getFiles = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getFiles");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files files = Server::getFiles(searchPath);
    size_t count = 0;
    for (auto item : files)
    {
      std::string countStr = Utilities::Converter<size_t>::toString(++count);
      reply.attribute("file" + countStr, item);
    }
  }
  else
  {
    std::cout << "\n  getFiles message did not define a path attribute";
  }
  return reply;
};

//----< Procedure that executes when client sends a getDirs command >------------------

std::function<Msg(Msg)> getDirs = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getDirs");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files dirs = Server::getDirs(searchPath);
    size_t count = 0;
    for (auto item : dirs)
    {
      if (item != ".." && item != ".")
      {
        std::string countStr = Utilities::Converter<size_t>::toString(++count);
        reply.attribute("dir" + countStr, item);
      }
    }
  }
  else
  {
    std::cout << "\n  getDirs message did not define a path attribute";
  }
  return reply;
};

//----< Procedure that executes when client sends a publish command >------------------

std::function<Msg(Msg)> publish = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("publish");
	std::string path = storageRoot + "\\" + msg.value("path");
	std::string subdir = msg.value("subdir");
	if (path != "") {
		std::vector<std::string> commandLine;
		commandLine.push_back("ServerPrototype.exe");
		commandLine.push_back(path);
		if (subdir != "") commandLine.push_back(subdir);
		int i = 0;
		while (msg.value("pattern" + std::to_string(i)) != "") {
			std::string pattern = msg.value("pattern" + std::to_string(i));
			commandLine.push_back(pattern);
			i++;
		}
		i = 0;
		while (msg.value("regex" + std::to_string(i)) != "") {
			std::string regex = msg.value("regex" + std::to_string(i));
			commandLine.push_back(regex);
			i++;
		}
		int argc = commandLine.size();
		char ** argv = new char*[argc];
		for (int i = 0; i < argc; i++) {
			std::string arg = commandLine[i];
			argv[i] = new char[commandLine[i].size() + 1];
			strcpy_s(argv[i], commandLine[i].size() + 1, arg.c_str());
		}
		Executive exeObj;
		auto convFiles = exeObj.executePublisher(argc, argv);
		auto depFiles = exeObj.getDepVect();
		delete[] argv;
		size_t count = 0;
		if (convFiles.size() == 0) reply.attribute("error0", "nofile");
		for (auto item : convFiles) {
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("file" + countStr, item);
		}
		for (auto item : depFiles) {
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("depFile" + countStr, item);
		}
	}
	else std::cout << "\n  publish message did not define a path attribute";
	return reply;
};

//----< Procedure that executes when client sends a download command >------------------

std::function<Msg(Msg)> download = [](Msg msg) {
	
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("file");

	std::string fileName = msg.value("name");
	reply.attributes()["file"] = fileName;
	
	std::string download_path = msg.value("path");
	reply.attributes()["path"] = download_path;
	
	reply.file(reply.attributes()["file"]);

	return reply;
};


int main(int argc, char ** argv)
{
	std::cout << ("\n\nREQUIREMENT 3 --------- Assembled working parts from Projects #1, #2, and #3 into a Client-Server configuration ---------\n");

	std::string rootDirectory = argv[1];
	std::string serverUrl = argv[2];
	int serverPort = std::stoi(argv[3]);

	const MsgPassingCommunication::EndPoint serverEndPoint(serverUrl, serverPort);
	storageRoot = rootDirectory;

	Server server(serverEndPoint, "ServerPrototype");
	server.start();

	std::cout << "\n  testing getFiles and getDirs methods";
	std::cout << "\n --------------------------------------";
	Files files = server.getFiles();
	show(files, "Files:");
	Dirs dirs = server.getDirs();
	show(dirs, "Dirs:");
	std::cout << "\n";

	std::cout << "\n  testing message processing";
	std::cout << "\n ----------------------------";
	server.addMsgProc("echo", echo);
	server.addMsgProc("getFiles", getFiles);
	server.addMsgProc("getDirs", getDirs);
	server.addMsgProc("serverQuit", echo);
	server.addMsgProc("publish", publish);
	server.addMsgProc("download", download);
	server.processMessages();
  
	std::cout << "\n  press enter to exit";
	std::cin.get();
	std::cout << "\n";

	server.stop();
	return 0;
}


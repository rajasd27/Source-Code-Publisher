# Source-Code-Publisher

## Overview
The source code publisher is a prototype that allows the user to view C++ header and source files from a remote server into the user's browsers and download the files if required. It also provides information about the dependencies of a particular C++ file and also provides user friendly functions which allows the users to hide comments, check function, class and namespace scopes.

## Running the Source Code Publisher

- Open the *run.bat* file in a text editor. 
- Start commands for both server and clients is provided. You can change the Port Numbers in both the commands if required. Otherwise, keep the defaults and save the file.
- After saving, double click on the *run.bat* file to run the application.
- After opening, the application will prompt you with a message asking you to view an automated test.
- If you press **No**, you will see all the files stored on the server. (In this case the project directory is considered as the directory stored on the server)
- Provide a pattern and/or a regular expression to match the files which you want from the server. Hit convert and in the next Tab you will see all the files matching the provided pattern and/or a regular expression converted to HTML format so that you can view it on the browser. Select the file to download locally or double click on the file to download it locally.

## Code Services 

The parts in this demo are:
- GUI (C# WPF Application)
  - Demonstrates C# user interface sending messages through native C++ communication channel to a remote server.
  - Connects to channel through Translater
- Translater (C++\CLI dll)
  - Defines managed message class
  - Translates managed messages and strings to native messages and strings and vice versa.
  - Connects to channel via the CommLibWrapper.
  - Managed code (C++\CLI) is isolated from native code (C++) using IComm interface and CommLibWrapper factory
- MessagePassingComm (C++)
  - Sends messages through socket-based one-way channels
  - Sends files as a series of chunks with message headers
  - Each endpoint has both a sender and receiver, wrapped in a Comm object
- Sockets (C++)
  - Socket library the supports both IP4 and IP6 protocols
  - Provides three classes Socket, SocketConnector, and SocketListener
- Message (C++)
  - Defines messages as collections of attributes
- ServerPrototype (C++)
  - Processes incoming messages based on ServerProc callable objects, keyed to a message's command attribute
- FileSystem (C++)
  - Provides static functions for managing files, paths, and directories


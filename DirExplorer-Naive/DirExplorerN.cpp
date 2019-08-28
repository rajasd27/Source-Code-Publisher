//////////////////////////////////////////////////////////////////////////
// DirExplorerN. - Naive directory explorer                             //
// ver 1.5                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Author:      Rajas Deshpande, Syracuse University                    //
//              rdeshpan@syr.edu                                        //
// Source:      Jim Fawcett, Syracuse University, CST 4-187             //
//              jfawcett@twcny.rr.com                                   //
//////////////////////////////////////////////////////////////////////////

#ifdef TEST_DIREXPLORERN  // only compile the following when defined

#include "DirExplorerN.h"
#include "../Utilities/Utilities.h"
#include "../Utilities/Utilities.h"

using namespace Utilities;
using namespace FileSystem;

ProcessCmdLine::Usage customUsage()
{
  std::string usage;
  usage += "\n  Command Line: path [/option]* [/pattern]*";
  usage += "\n    path is relative or absolute path where processing begins";
  usage += "\n    [/option]* are one or more options of the form:";
  usage += "\n      /s - walk directory recursively";
  usage += "\n    [pattern]* are one or more pattern strings of the form:";
  usage += "\n      *.h *.cpp *.cs *.txt or *.*";
  usage += "\n";
  return usage;
}

int main(int argc, char *argv[])
{
  Title("Demonstrate DirExplorer-Naive, " + DirExplorerN::version());

  ProcessCmdLine pcl(argc, argv);
  pcl.usage(customUsage());

  preface("Command Line: ");
  pcl.showCmdLine();
  putline();

  if (pcl.parseError())
  {
    pcl.usage();
    std::cout << "\n\n";
    return 1;
  }

  DirExplorerN de(pcl.path());
  for (auto patt : pcl.patterns())
  {
    de.addPattern(patt);
  }

  if (pcl.hasOption('s'))
  {
    de.recurse();
  }

  de.search();
  de.showStats();

  de.getVector();

  std::cout << "\n\n";
  return 0;
}

#endif
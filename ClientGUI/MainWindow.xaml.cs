/////////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs : WPF Client that uses Shim to call native C++ code  //
// ver 1.1                                                                 //
// Application   : Spring 2019 Project 3, CSE687 - Object Oriented Design  //
// Platform      : Visual Studio 2017 Community Edition                    //
// Author        : Rajas Deshpande, Syracuse University                    //
//                 rdeshpan@syr.edu                                        //
/////////////////////////////////////////////////////////////////////////////
/*
 *  Package Decription:
 * =====================
 *  This package defines the GUI using MainWindow.xaml file which provides
 *  the working of the Code Publisher. This package depends on Shim project, 
 *  if Shim project is NOT built then Shim.dll does not exist and therefore 
 *  you'll get Intellisense error saying you have missing reference. Do not 
 *  worry about the error, once Shim project is successfully built this will 
 *  build and run.
 *  
 *  Public Interface: N/A
 *  
 *  Required Files:
 * =================
 *  MainWindow.xaml MainWindow.xaml.cs Translator.dll
 *  
 *  Build Process:
 * ================
 *  msbuild ClientGUI.csproj
 *  
 *  Maintainence History:
 * =======================
 *  ver 1.1 - April 08th, 2019
 *    - Added Client-Server Communication by defining endpoints and functionalities
 *  ver 1.0 - April 08th, 2019
 *    - first release
 */


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Diagnostics;
using System.IO;
using System.Text.RegularExpressions;
using Path = System.IO.Path;
using System.Windows.Threading;
using MsgPassingCommunication;

namespace ClientGUI
{
    public partial class MainWindow : Window
    {

        public MainWindow()
        {
            InitializeComponent();
        }

        private string Working_dir { get; set; }
        private string Download_dir { get; set; }
        private string Pattern { get; set; }
        private string Regexes { get; set; }
        private bool CheckBox { get; set; }
        private List<string> FilesMatchingPattern = new List<string>();
        private List<string> CommandLine = new List<string>();
        string serverRootDirectory;
        string url;
        int serverPort;
        int clientPort;


        private Stack<string> pathStack_ = new Stack<string>();
        private Translater translater;
        private CsEndPoint endPoint_;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_
          = new Dictionary<string, Action<CsMessage>>();

        //----< process incoming messages on child thread >----------------

        private void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    string msgId = msg.value("command");
                    if (dispatcher_.ContainsKey(msgId))
                        dispatcher_[msgId].Invoke(msg);
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }

        //----< function dispatched by child thread to main thread >-------

        private void clearDirs()
        {
            lstDirs.Items.Clear();
        }

        //----< function dispatched by child thread to main thread >-------

        private void addDir(string dir)
        {
            lstDirs.Items.Add(dir);
        }

        //----< function dispatched by child thread to main thread >-------

        private void insertParent()
        {
            lstDirs.Items.Insert(0, "..");
        }

        //----< function dispatched by child thread to main thread >-------

        private void clearFiles()
        {
            lstFiles.Items.Clear();
        }

        //----< function dispatched by child thread to main thread >-------

        private void addFile(string file)
        {
            lstFiles.Items.Add(file);
        }

        //----< function dispatched by child thread to main thread >-------

        private void addDownloadFile()
        {
            downFiles.Items.Clear();
            Display.Text = "File Downloaded";
            FilesMatchingPattern.Clear();
            var vect = Directory.GetFiles(Download_dir);
            if (vect.Length != 0) DownLabel.Content = "Downloaded Files : [ Double Click to open any file ]";
            FilesMatchingPattern.AddRange(vect);
            foreach (var file in FilesMatchingPattern) downFiles.Items.Add(Path.GetFileName(file));
        }

        //----< function dispatched by child thread to main thread >-------

        private void addFileDisplay(string file)
        {
            if(!convFiles.Items.Contains(Path.GetFileName(file) + ".html"))
                convFiles.Items.Add(Path.GetFileName(file)+".html");

            if (convFiles.Items.Count == 0)
            {
                MessageBox.Show("Server Did not find any files. Please check the pattern or regex", "Error");
                statusText.Text = "No Files Converted....";
                Display.Text = "No Files Converted....";
            }

            else
            {
                convLabel.Content = "Converted Files : [ Double Click to download any file from the server ]";
                statusText.Text = "Files Successfully Converted....";
                Display.Text = "Conversion Successful";
            }               
        }

        //----< function dispatched by child thread to main thread >-------

        private void addDepFileDisplay(string file)
        {
            file = Path.GetFullPath(file);
            if (!depFiles.Items.Contains(Path.GetFileName(file)))
                depFiles.Items.Add(Path.GetFileName(file));

            if (depFiles.Items.Count == 0)
            {
                MessageBox.Show("No Dependency files found.", "Notification");
            }

            else
            {
                depLabel.Content = "Dependencies : [ Double Click to download any file from the server ]";
            }

        }

        //----< function dispatched by child thread to main thread >-------

        private async void displayMessage()
        {
            MessageBox.Show("Connected to the Server\nServer Address : " + url + "/" + serverPort + "\nClient Address : " + 
                url + "/" + clientPort, "Connection Successful");
            int res = await initAutomatedTest();
        }

        //----< function dispatched by child thread to main thread >-------

        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }

        //----< load getDirs processing into dispatcher dictionary >-------

        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
                {
                    clearDirs();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                int flag = 0;
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        flag = 1;
                        Action<string> doDir = (string dir) =>
                        {
                            
                            addDir(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }

                if(flag == 0)
                {
                    MessageBox.Show("No Directories Found", "Notification");
                }
                 
                Action insertUp = () =>
                {
                    insertParent();
                };
                Dispatcher.Invoke(insertUp, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }

        //----< load getFiles processing into dispatcher dictionary >------

        private void DispatcherLoadGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    clearFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                int flag = 0;
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        flag = 1;
                        Action<string> doFile = (string file) =>
                        {
                            addFile(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }                    
                }

                if(flag == 0)
                {
                    MessageBox.Show("No Files Found", "Notification");
                }
            };
            addClientProc("getFiles", getFiles);
        }

        //----< load DisplayGetFiles processing into dispatcher dictionary >------

        private void DispatcherDisplayGetFiles()
        {
            Action<CsMessage> publish = (CsMessage rcvMsg) =>
            {
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addFileDisplay(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }

                    if (key.Contains("depFile"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addDepFileDisplay(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                    
                    if (key.Contains("error"))
                    {

                        Action<string> doFile = (string file) =>
                        {
                            MessageBox.Show("No files found to convert", "Notification");
                            Display.Text = "Ready...";
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("publish", publish);
        }

        //----< load DownloadGetFiles processing into dispatcher dictionary >------

        private void DispatcherDownloadGetFiles()
        {
            Action<CsMessage> download = (CsMessage rcvMsg) =>
            {
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            addDownloadFile();
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("file", download);
        }

        //----< load initserver processing into dispatcher dictionary >------

        private void DispatcherinitServer()
        {
            Action<CsMessage> init = (CsMessage rcvMsg) =>
            {
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("command"))
                    {
                        Action<string> doInit = (string file) =>
                        {
                            displayMessage();
                        };
                        Dispatcher.Invoke(doInit, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("echo", init);
        }

        //----< load all dispatcher processing >---------------------------
        private void loadDispatcher()
        {
            DispatcherinitServer();
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherDisplayGetFiles();
            DispatcherDownloadGetFiles();
        }

        //----< Send message to check whether the server is connected to the client >------

        private void init()
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = url;
            serverEndPoint.port = serverPort;

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "echo");
            translater.postMessage(msg);
        }

        //----< Initializes all the client data and processes command line >------

        private void load()
        {
            convFiles.Items.Clear();
            depFiles.Items.Clear();
            Pattern = "*.h *.cpp";
            textPatt.Text = Pattern;
            textRegex.Text = "^[A-D](.*)";

            Download_dir = "../../../../SaveFiles";
            Download_dir = Path.GetFullPath(Download_dir);
            textDownload.Text = Download_dir;
            addDownloadFile();
            if (downFiles.Items.Count != 0)
                Display.Text = "Previously Downloaded Files";

            string[] args = Environment.GetCommandLineArgs();
            serverRootDirectory = args[1];
            url = args[2];
            serverPort = int.Parse(args[3]);
            clientPort = int.Parse(args[4]);
        }

        //----< start Comm, fill window display with dirs and files >------

        private void Window_Loaded(object sender, RoutedEventArgs e) 
        {
            Console.Write("\n\nREQUIREMENT 2 --------- Using WPF and C# to implement the Client GUI ---------\n");
            Console.Write("\n\nREQUIREMENT 3 --------- Assembled working parts from Projects #1, #2, and #3 into a Client-Server configuration ---------\n");
            Console.Write("\n\nREQUIREMENT 4 --------- Shall provide a Graphical User Interface(GUI) for the client that\n");
            Console.Write("supports navigating remote directories to find a project for conversion, and supports ---------\n");
            Console.Write("displaying the conversion results in a way that meets Project #3 requirements. ---------\n");
            Console.Write("\n\nREQUIREMENT 7 --------- Demonstrating correct operations for two or more concurrent clients. ---------\n");

            load();

            if (serverPort == clientPort) MessageBox.Show("Server Port and Client Port cannot be equal", "Error");
            else
            {
                endPoint_ = new CsEndPoint();
                endPoint_.machineAddress = "localhost";
                endPoint_.port = clientPort;
                translater = new Translater();
                translater.listen(endPoint_);

                processMessages();                  // start processing messages               
                loadDispatcher();                   // load dispatcher
                init();
                
                CsEndPoint serverEndPoint = new CsEndPoint();
                serverEndPoint.machineAddress = url;
                serverEndPoint.port = serverPort;

                textPath.Text = removeFirstDir(serverRootDirectory);
                pathStack_.Push(serverRootDirectory);

                Console.Write("\n\nREQUIREMENT 5 : --------- Providing message design for getting files and directories ---------\n");

                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "getDirs");
                msg.add("path", pathStack_.Peek());
                translater.postMessage(msg);
                msg.remove("command");
                msg.add("command", "getFiles");
                translater.postMessage(msg);
            }
        }

        //----< Function of execute the automated test >------

        private async Task<int> initAutomatedTest()
        {
            Console.Write("\n\nREQUIREMENT 7 --------- Includes an automated test that accepts the server url and remote path to your project directory\n");
            Console.Write("on its command line, invokes the Code Publisher, through its interface, to convert all the project files matching a\n");
            Console.Write("pattern that accepts *.h and *.cpp files, and then opens the Client GUI's Display view.---------\n");

            await Task.Delay(500);
            MessageBoxResult result =  MessageBox.Show("Do you want to initiate the automated test?", "Automated Test", MessageBoxButton.YesNo);
            if(result == MessageBoxResult.Yes)
            {
                BtnPublish_Click(null, null);
                return 0;
            }

            else
            {
                return 0;
            }
        }

        //----< Update Variable when regex is changed >------

        private void TextRegex_TextChanged(object sender, RoutedEventArgs e)
        {
            Regexes = textRegex.Text.ToString();
        }


        private string removeFirstDir(string path)
        {
            string modifiedPath = path;
            int pos1 = path.IndexOf("/");
            modifiedPath = path.Substring(pos1 + 1, path.Length - pos1 - 1);

            string finalPath = modifiedPath;
            int pos2 = modifiedPath.IndexOf("/");
            finalPath = modifiedPath.Substring(pos2 + 1, modifiedPath.Length - pos2 - 1);

            return finalPath;
        }

        //----< respond to mouse double-click on dir name >----------------

        private void LstDirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            Display.Text = "Ready..";
            statusText.Text = "Ready..";
            string selectedDir = (string)lstDirs.SelectedItem;
            if (selectedDir == null)
                return;
            string path;
            if (selectedDir == "..")
            {
                if (pathStack_.Count > 1)  // don't pop off "Project4"
                    pathStack_.Pop();
                else
                    return;
            }
            else
            {
                path = pathStack_.Peek() + "/" + selectedDir;
                pathStack_.Push(path);
            }

            textPath.Text = removeFirstDir(pathStack_.Peek());

            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = url;
            serverEndPoint.port = serverPort;

            Console.Write("\n\nREQUIREMENT 5 : --------- Providing message design for getting files and directories ---------\n");

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "getDirs");
            msg.add("path", pathStack_.Peek());
            translater.postMessage(msg);

            // build message to get files and post it
            msg.remove("command");
            msg.add("command", "getFiles");
            translater.postMessage(msg);
        }

        //----< Push button to send message to server to receive converted files >----------------

        private async void BtnPublish_Click(object sender, RoutedEventArgs e) 
        {
            Console.Write("\n\nREQUIREMENT 5 : --------- Providing message design for requesting converted files ---------\n");
            Console.Write("\n\nREQUIREMENT 6 : --------- Supports converting source code in the server with a separate request ---------\n");

            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = url;
            serverEndPoint.port = serverPort;

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "publish");
            msg.add("path", pathStack_.Peek());

            if (textPatt.Text == "") MessageBox.Show("Enter a valid Pattern", "Error");
            else
            {
                var pat = textPatt.Text.Split();
                for (var i=0; i<pat.Count(); i++)
                {
                    msg.add("pattern" + i, pat[i]);
                }
            }

            if(textRegex.Text != "")
            {
                var re = textRegex.Text.Split();
                for (var i = 0; i < re.Count(); i++)
                {
                    msg.add("regex" + i, re[i]);
                }
            }
            
            if (chkbox.IsChecked.Value) msg.add("subdir", "/s");
            else msg.add("subdir", "");

            if(textPatt.Text != "") translater.postMessage(msg);
            statusText.Text = "Converting Files....";

            await Dispatcher.BeginInvoke((Action)(() =>
            {
                Display.Text = "Conversion In progress...";
                statusText.Text = "Ready...";
                tabCtrl1.SelectedIndex = 1;
            }));
        }

        //----< respond to mouse double-click on converted files to download them on the client side >----------------

        private void ConvFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            Console.Write("\n\nREQUIREMENT 6 : --------- Transferring one or more converted files back to the local client, using the communication channel ---------\n");
    

            Display.Text = "Downloading File..";
            string selectedFile = (string)convFiles.SelectedItem;
            MessageBox.Show(selectedFile + " File will get downloaded from the Server", "Downloading");

            if (convFiles.SelectedIndex == -1) return;

            Console.Write("\n\nREQUIREMENT 5 : --------- Providing message design for downloading files ---------\n");

            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = url;
            serverEndPoint.port = serverPort;

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "download");
            msg.add("name", selectedFile);
            msg.add("path", Download_dir);
            translater.postMessage(msg);
        }

        //----< respond to mouse double-click on file name to open all files in browser >----------------

        private async void BtnDisplay_Click(object sender, RoutedEventArgs e) 
        {
            for (int i = 0; i < downFiles.Items.Count; i++)
            {
                try
                {
                    var html = downFiles.Items[i].ToString();
                    html = Download_dir + "/" + html;
                    var p = System.Diagnostics.Process.Start(Path.GetFullPath(html));
                    while (!p.HasExited)
                        await Task.Delay(500);
                }
                catch
                {
                    continue;
                }
            }
        }

        //----< respond to mouse double-click on file name to display in browser or text editor>----------------

        private void DownFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (downFiles.SelectedIndex == -1) return;

            string selectedItem = downFiles.SelectedItem.ToString();
            selectedItem = Download_dir+ "/" + selectedItem;
            selectedItem = Path.GetFullPath(selectedItem);

            if (!webCheck.IsChecked.Value && !sourceCheck.IsChecked.Value)
                MessageBox.Show("Please check one of the display options", "Error");

            else
            {
                if (webCheck.IsChecked.Value) System.Diagnostics.Process.Start(selectedItem);
                if (sourceCheck.IsChecked.Value) System.Diagnostics.Process.Start("notepad.exe", selectedItem);
            }
            if (!Directory.Exists(Path.GetFullPath(selectedItem))) return;
        }

        //----< browse file systems to select a directory to store files >----------------

        private void BtnDown_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.FolderBrowserDialog dialog;
            dialog = new System.Windows.Forms.FolderBrowserDialog();

            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                Download_dir = dialog.SelectedPath;
                textDownload.Text = Download_dir;
            }
        }

        private void TextDownload_TextChanged(object sender, TextChangedEventArgs e)
        {
            addDownloadFile();
        }

        private void DepFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            Display.Text = "Downloading File..";
            string selectedFile = (string)depFiles.SelectedItem;
            MessageBox.Show(selectedFile + " File will get downloaded from the Server", "Downloading");

            if (depFiles.SelectedIndex == -1) return;

            Console.Write("\n\nREQUIREMENT 5 : --------- Providing message design for downloading files ---------\n");

            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = url;
            serverEndPoint.port = serverPort;

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "download");
            msg.add("name", selectedFile);
            msg.add("path", Download_dir);
            translater.postMessage(msg);
        }
    }
}
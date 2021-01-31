//
// Copyright (C) 2021 ExL Studios
//
//
/***********************************************************************************************
 *               C L A S S I F I E D --- ( C )   2 0 2 1   E x L   S T U D I O S               *
 ***********************************************************************************************
 *                                                                                             *
 *                              Project Name :   MCDream                                       *
 *                                                                                             *
 *                              File Name    :   MCDream.cpp                                   *
 *                                                                                             *
 *                              Programmer   :   ExecuteIf                                     *
 *                                                                                             *
 *                              Start Date   :   2020/01/11                                    *
 *                                                                                             *
 *                              Last Update  :   2020/01/31                                    *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *             UTC(void)                                                                       *
 * Classes:                                                                                    *
 *             server                                                                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// decomment to turn on gcc optimize 3
//#pragma GCC optimize(3)

#include "main.hpp"

// vars & types
HWND hwnd = FindWindow("ConsoleWindowClass",NULL);
bool EnableLogs = true;
bool debugmode = false;
bool OM = false; // optimize
bool ae = true;
bool nogui = false;
int cfgTotalLine = 0;
char title[21] = "title \0\0\0\0\0\0\0\0\0\0\0\0\0\0";
char cfg[101][10001];
char Xmn[101]="\0";
char Xmx[101]="\0";

// functions
char* UTC() // get time
{
	time_t now = time(0);
	char* dt = ctime(&now);
	tm *gmtm = gmtime(&now);
	dt = asctime(gmtm);
	return dt;
}

class servers // server launcher
{
	public:
		bool optimize;
		bool autoEula;
		bool ngui;
		char RamMin[101];
		char RamMax[101];
		char commandline[10001];
		void launch()
		{
			/* DONE (Executeif#1#): Launch */
			this->checkserverjar();
			this->eulaprocess();
			this->commandlinecutter();
			if(debugmode)
			{
				system("pause");
			}
			this->execute();
		}
		void backup()
		{
			system("md backups");
			system("md temp");
			system("xcopy .\\resources\\bu.mcd .\\temp /q /y");
			system("ren .\\temp\\*.mcd *.bat");
			system(".\\temp\\bu.bat");
			cout<<"Successfully created backup.\n";
		}
		void launchscript()
		{
			system("cd.>Script-LaunchServer.cmd");
			fstream scriptOut;
			scriptOut.open(".\\Script-LaunchServer.cmd");
			if(!scriptOut.is_open())
			{
				cout<<"[Error] Failed to create script--Permission Denied.\n";
				return;
			}
			scriptOut<<"@echo off\n";
			this->commandlinecutter();
			for(int i=0;i<strlen(commandline);i++)
			{
				scriptOut<<commandline[i];
			}
			scriptOut<<"\necho Server stopped.\necho Press any key to continue...\npause >nul\nexit\n";
			scriptOut<<flush;
			scriptOut.close();
			cout<<"Created Launch script successfully.Go .\\Script-LauncherServer.cmd for more info.\n";
		}
	private:
		void eulaprocess()
		{
			/* DONE (Executeif#1#): AutoEula */
			if(autoEula)
			{
				ifstream eulatest;
				eulatest.open(".\\server\\eula.txt");
				if(!eulatest.is_open())
				{
					if(debugmode)
					{
						cout<<"[DEBUG]Can not read eula.Creating new...\n";
					}
					eulatest.close();
					system("xcopy .\\resources\\server.mcd .\\server\\ /q /y");
					system("ren .\\server\\*.mcd *.properties");
					system("xcopy .\\resources\\eula.mcd .\\server\\ /q /y");
					system("ren .\\server\\*.mcd *.txt");
				}
			}
		}
		void checkserverjar()
		{
			ifstream servertest;
			servertest.open(".\\server\\server.jar");
			if(!servertest.is_open())
			{
				cout<<"[ERROR]Can not find server.jar!\nput your server.jar in .\\server\\ !\n";
				servertest.close();
				exit(3);
			}
		}
		void commandlinecutter()
		{
			memset(commandline,'\0',sizeof(commandline)); // Initialize <commandline>
			
			strcpy(commandline,"cd .\\server\\ && java -jar ");
			
			//Xmn
			strcat(commandline,"-Xmn");
			strcat(commandline,RamMin);
			strcat(commandline," ");
			
			//Xmx
			strcat(commandline,"-Xmx");
			strcat(commandline,RamMax);
			strcat(commandline," ");
			
			//if(optimize) // optimize
			//{
			//	strcat(commandline,"-XX:+AggressiveOpts -XX:+UseCompressedOops -XX:+UseCMSCompactAtFullCollection -XX:+UseFastAccessorMethods -XX:ParallelGCThreads=4 -XX:+UseConcMarkSweepGC -XX:CMSFullGCsBeforeCompaction=2 -XX:CMSInitiatingOccupancyFraction=70 -XX:-DisableExplicitGC -XX:TargetSurvivorRatio=90");
			//}
			strcat(commandline," .\\server.jar");
			
			if(!ngui)
			{
				strcat(commandline," nogui");
			}
		}
		void execute()
		{
			if(debugmode)
			{
				cout<<"\n[CLN] commandline <- ";
				puts(commandline);
				puts("\n================================================\n");
			}
			system(commandline);
		}
};

// main
int main(int argc, char* argv[])
{
	system("del /s /f /q .\\temp\\bu.bat");
	system("cls");
	/* DONE (ExecuteIf#1#): Set-up */
	system("title ControlPlane-MCDream");
	system("color 2F"); // | green | with white
	/* DONE (ExecuteIf#1#): Log-Process */
	// prepare for log-output
	ofstream log;
	log.open(".\\latest.log", ios::app);
	if(!log.is_open())
	{
		MessageBox(hwnd,"[STDERR]::Failed write log!--Permission Denied.","ERROR-MCDream",MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
		return 1;
	}
	
	/* DONE (ExecuteIf#1#): Config-Process */
	// input config
	ifstream config;
	config.open(".\\config.properties");
	if(!config.is_open())
	{
		system("xcopy .\\resources\\cfg.mcd .\\ /q /y");
		system("ren .\\cfg.mcd config.properties");
		log<<UTC()<<"[STDERR]\tNo!I can not read config (TxT)\n"<<endl;
		MessageBox(hwnd,"It looks like you're running mcdream for the first time -- but don't be worry about it, we'll help you create the necessary files.\nJust execute these 4 steps(check \"OK\" to continue)","ERROR-MCDream",MB_OK | MB_SYSTEMMODAL);
		system("start https://java.com/");
		MessageBox(hwnd,"Firstly,download and install java!","MCDream",MB_OK | MB_SYSTEMMODAL);
		system("start");
		MessageBox(hwnd,"If you can run command \"java\" in cmd,you installed java successfully.If not,you should try install again.Or the Server will not work.","MCDream",MB_OK | MB_SYSTEMMODAL);
		system("taskkill /t /f /im cmd.exe");
		system("md server");
		system("start .\\server\\");
		MessageBox(hwnd,"Next,please put your server.jar to .\\server\\ ,and make sure its name is \"server.jar\"","MCDream",MB_OK | MB_SYSTEMMODAL);
		system("start notepad .\\config.properties");
		system("start https://github.com/MC-Dream/MCDream-x64-for-windows/blob/main/config.md");
		MessageBox(hwnd,"Then,please Edit \"config.properties\" as the website say.\n","MCDream",MB_OK | MB_SYSTEMMODAL);
		MessageBox(hwnd,"Finally,restart MCDream.\nEverything will be better...\n","MCDream",MB_OK | MB_SYSTEMMODAL);
		log<<"Program ended\n=======================\n\n";
		log<<flush;
		log.close();
		return 2;
	}
	char temp[1001];
	int tempjjjj = 0;
	for(; !config.eof(); tempjjjj++)
	{
		memset(temp,'\0',sizeof(temp));
		config.getline(temp,sizeof(temp));
		if(temp[0]!='#' && strlen(temp)>1)
		{
			memset(cfg[cfgTotalLine],'\0',sizeof(cfg[cfgTotalLine]));
			strcpy(cfg[cfgTotalLine],temp);
			cfgTotalLine += 1;
		}
	}

	for(int i=0; i<=cfgTotalLine; i++) // apply config
	{
//		puts(cfg[i]);
		if(!strncmp(cfg[i],"debug",5))
		{
			if(cfg[i][6]=='t')
			{
				debugmode=true;
				cout<<"Debugmode <- on\n";
				log<<UTC()<<"[INFO]\tDebugmode <- on\n";
			}
			else if(cfg[i][6]=='f')
			{
//				ShowWindow(hwnd,SW_HIDE); // hide window
				debugmode=false;
				log<<UTC()<<"[INFO]\tDebugmode <- off\n";
			}
		}
		else if(!strncmp(cfg[i],"Xmn",3))
		{
			int k=0;
			if(debugmode)
			{
				cout<<"Xmn= <- ";
			}
			log<<UTC()<<"[INFO]\tXmn <- ";
			for(int j=4; j<=strlen(cfg[i]); j++) // write Xmn to <Xmn>
			{
				if(debugmode)
				{
					cout<<cfg[i][j];
				}
				log<<cfg[i][j];
				Xmn[k]=cfg[i][j];
				k++;
			}
			if(debugmode)
			{
				puts(""); // endl
			}
			log<<"\n";
		}
		else if(!strncmp(cfg[i],"Xmx",3))
		{
			if(debugmode)
			{
				cout<<"Xmx= <- ";
			}
			log<<UTC()<<"[INFO]\tXmx <- ";
			int o=0;
			for(int j=4; j<=strlen(cfg[i]); j++) // write Xmx to <Xmx>
			{
				if(debugmode)
				{
					cout<<cfg[i][j];
				}
				log<<cfg[i][j];
				Xmx[o]=cfg[i][j];
				o++;
			}
			if(debugmode)
			{
				puts(""); // endl
			}
			log<<"\n";
		}
		else if(!strncmp(cfg[i],"optimize",8))
		{
			if(debugmode)
			{
				cout<<"optimize <- true\n";
			}
			log<<UTC()<<"[INFO]\toptimize <- true\n";
			OM=true;
		}
		else if(!strncmp(cfg[i],"AutoEula",8))
		{
			if(cfg[i][9]=='t')
			{
				if(debugmode)
				{
					cout<<"AutoEula <- true\n";
				}
				log<<UTC()<<"[INFO]\tAutoEula <- true\n";
				ae=true;
			}
			else
			{
				ae=false;
				log<<UTC()<<"[INFO]\tAutoEula <- false\n";
			}
		}
		else if(!strncmp(cfg[i],"nogui",5))
		{
			if(cfg[i][6]=='t')
			{
				if(debugmode)
				{
					cout<<"Nogui <- true\n";
				}
				log<<UTC()<<"[INFO]\tNogui <- true\n";
			}
			else
			{
				if(debugmode)
				{
					cout<<"Nogui <- false\n";
				}
				log<<UTC()<<"[INFO]\tNogui <- false\n";
			}
		}
	}
	if(debugmode)
	{
		cout<<"Get config.\n";
	}
	cout<<"Started the server.";
	cout<<"Press Ctrl+C to exit!\n";
	log<<UTC()<<"[INFO]\tGet config.I'm gonna launch the server...\n"; // got config
	
	/* DONE (ExecuteIf#1#): Launcher */
	servers awa; // prepare for launch
	awa.optimize=OM;
	awa.ngui=nogui;
	strcpy(awa.RamMin,Xmn);
	strcpy(awa.RamMax,Xmx);
	awa.autoEula=ae;
	
	/* TODO (ExecuteIf#1#): User interaction */
	if(debugmode)
	{
		system("pause");
	}
	while(1)
	{
		system("cls");
		cout<<"\n\n";
		cout<<"What do you want to do?\n";
		cout<<"+---------------+------------------------------------------+\n";
		cout<<"| Serial number |                 WHAT TO DO               |\n";
		cout<<"+---------------+------------------------------------------+\n";
		cout<<"|       1       |            Launch the server             |\n";
		cout<<"|       2       |      Create a backup for the server      |\n";
		cout<<"|       3       | Create a script for launching the server |\n";
		cout<<"|       4       |                    Exit                  |\n";
		cout<<"+----------------------------------------------------------+\n";
		cout<<"enter the serial number to choose:";
		int tmp;
		cin>>tmp;
		switch(tmp)
		{
			case 1:
				awa.launch(); // it's time!Lunch now!
				log<<UTC()<<"[INFO]Server started successfully!\n";
				system("pause");
				break;
			case 2:
				awa.backup(); // back up the server!
				cout<<"Go .\\backups\\backup-<time>\\ for more info!\n";
				log<<UTC()<<"[INFO]Server backuped successfully!\n";
				system("pause");
				break;
			case 3:
				awa.launchscript();
				system("pause");
				break;
			case 4:
				goto out;
			default:
				system("start https://github.com/MC-Dream/MCDream-x64-for-windows/issues");
				MessageBox(hwnd,"MCDream Crashed!\nPlease visit our issue tracker to report the bug!","ERROR-MCDream",MB_OK | MB_SYSTEMMODAL);
				exit(-1);
		}
	}
	
	out:
	// flush&close log system
	//	log<<"Oh please don't let me die-Waiting for your touch-No, don't give up on life-This endless dead end-锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷\n=======================\n\n";
	log<<"Process ended\n=======================\n\n";
	log<<flush;
	log.close();
	system("pause");
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include "backup.h"
#include "transfer.h"
#include "paths.h"
#include "permissions.h"

// signal handler
void signalhandler(int sigNum)
{
	if (sigNum == SIGINT)
	{
		prepareforexit();
		syslog(LOG_INFO, "Exiting daemon");
		exit(EXIT_SUCCESS);
	}
}

void prepareforexit()
{
	// prepare for exit
	syslog(LOG_INFO, "Closing log");
	closelog();
}

int main()
{
	// set up timer
	time_t now;
    double seconds;
    time(&now);
    struct tm newtime;
    newtime = *localtime(&now);
    newtime.tm_hour = 18; 
    newtime.tm_min = 31; 
    newtime.tm_sec = 0;

	// create the child
	int pid = fork();
	
	if (pid > 0)
	{
		// end the parent
		sleep(10);
		exit(EXIT_SUCCESS);
	}
	else if (pid == 0)
	{
		// create the orphan

		// elevate to session leader and disconnect from TTY
		if (setsid() < 0)
		{
			exit(EXIT_FAILURE);
		}
		
		// set file creation mask to 0
		umask(0);
		
		// change working directory to root
		if (chdir("/") < 0)
		{
			exit(EXIT_FAILURE);
		}
		
		// close all open file descriptors
		for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
		{
			close(x);
		}

		// open syslog
		openlog("department-backup-daemon", LOG_PID | LOG_CONS, LOG_USER);

		// register signal handler
		if (signal(SIGINT, signalhandler) == SIG_ERR)
		{
			syslog(LOG_ERR, "Signal handler registration failed");
		}
		else
		{
			syslog(LOG_INFO, "Signal handler registration succeeded");
		}

		char folders[3][100] = {
			UPLOAD_DIR,
			REPORT_DIR,
			BACKUP_DIR
		};

		// create necessarry folders if they don't exist
		for (int i = 0; i < 3; i++)
		{
			if (access(folders[i], F_OK) != 0)
			{
				char command[100] = "mkdir "; 
				strcat(command, folders[i]);
				int status;
				if ((status = system(command)) != -1)
				{
					if (WEXITSTATUS(status) != 0)
					{
						syslog(LOG_ERR, "Failed to create folder: %s", folders[i]);
					}
					else
					{
						syslog(LOG_INFO, "Folder created: %s", folders[i]);
					}
				}
				else
				{
					syslog(LOG_ERR, "Folder creation process failed: %s", folders[i]);
				}
			}
		}

		// make sure backup and report directories are locked
		lockdir(BACKUP_DIR);
		lockdir(REPORT_DIR);

		// watch upload directory with auditctl
		system("auditctl -w " UPLOAD_DIR " -p rwxa");

		// keep daemon running with infinite loop
		while (1)
		{
			sleep(1);
			time(&now);
			seconds = difftime(now, mktime(&newtime));
			if (seconds == 0)
			{
				// lock upload folder during transfer and backup
				lockdir(UPLOAD_DIR);
				transfer();
				backup();
				unlockdir(UPLOAD_DIR);
			}
		}
	}
	
	return 0;
}

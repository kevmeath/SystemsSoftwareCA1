#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include "transfer.h"
#include "permissions.h"
#include "backup.h"
#include "paths.h"

int main()
{
	openlog("department-backup-daemon", LOG_PID | LOG_CONS, LOG_USER);

	char option = '0';
	
	do
	{
		printf("1. transfer\n");
		printf("2. backup\n");
		printf("3. log\n");
		printf("4. permissions\n");
		printf("0. quit\n");
		printf("Choose an option\n");
		scanf("%c", &option);
		getchar();
		
		switch (option)
		{
			case '1':
				transfer();
				break;
			case '2':
				backup();
				break;
			case '3':
				syslog(LOG_INFO, "TEST LOG");
			case '4':
				lockdir(UPLOAD_DIR);
				sleep(10);
				unlockdir(UPLOAD_DIR);
			case '0':
				break;
			default:
				printf("Invalid option\n");
				break;
		}
	} while (option != '0');
	
	closelog();

	return 0;
}

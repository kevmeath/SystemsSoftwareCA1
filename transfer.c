#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include "transfer.h"
#include "paths.h"

void transfer()
{
	// write access log
	system("ausearch -f " UPLOAD_DIR " > " LOG_DIR "accesslog.txt");

	syslog(LOG_INFO, "Starting transfer");

	// attempt transfer and get return status
    int status = system(TRANSFER_COMMAND " " UPLOAD_DIR "* " REPORT_DIR);

	// check return status and log appropriately
    if (status != -1)
    {
        if (WEXITSTATUS(status) != 0)
        {
            syslog(LOG_ERR, "Transfer failed");
        }
        else
        {
            syslog(LOG_INFO, "Transfer succeeded");
        }
    }
    else
    {
        syslog(LOG_ERR, "Transfer system process failed");
    }

	const char report_file_paths[FILE_COUNT][100] = {
		UPLOAD_DIR DISTRIBUTION_FILE,
		UPLOAD_DIR MANUFACTURING_FILE,
		UPLOAD_DIR SALES_FILE,
		UPLOAD_DIR WAREHOUSE_FILE
	};

	// check for the existance of each report file
	for (int i = 0; i < FILE_COUNT; i++)
	{
		if (access(report_file_paths[i], F_OK) == 0)
		{
			syslog(LOG_INFO, "Department report file found: %s", report_file_paths[i]);
		}
		else
		{
			syslog(LOG_INFO, "Department report file not found: %s", report_file_paths[i]);
		}
	}
}

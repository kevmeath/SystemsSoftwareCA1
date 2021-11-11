#include <stdlib.h>
#include <syslog.h>
#include "backup.h"
#include "paths.h"

void backup()
{
    syslog(LOG_INFO, "Starting backup");

    // attempt backup and get return status
    int status = system(ARCHIVE_COMMAND " " BACKUP_DIR BACKUP_FILE " " REPORT_DIR "*");

    // check status and log appropriately
    if (status != -1)
    {
        if (WEXITSTATUS(status) != 0)
        {
            syslog(LOG_ERR, "Backup failed");
        }
        else
        {
            syslog(LOG_INFO, "Backup succeeded");
        }
    }
    else
    {
        syslog(LOG_ERR, "Backup system process failed");
    }
}

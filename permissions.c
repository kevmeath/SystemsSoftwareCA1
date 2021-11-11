#include <stdlib.h>
#include <sys/stat.h>
#include <syslog.h>
#include "permissions.h"
#include "paths.h"

int changepermissions(char *, char *);

// change permissions for specified path
int changepermissions(char mode[], char path[])
{
    int i = strtol(mode, 0, 8);

    // apply permission value
    return chmod(path, i);
}

// disable all permissions for specified path
void lockdir(char path[])
{
    if (changepermissions("0444", path) == -1)
    {
        syslog(LOG_ERR, "Failed to lock: %s", path);
    }
    else
    {
        syslog(LOG_INFO, "Locked: %s", path);
    }
}

// enable all permissions for specified path
void unlockdir(char path[])
{
    if(changepermissions("0777", path) == -1)
    {
        syslog(LOG_ERR, "Failed to unlock: %s", path);
    }
    else
    {
        syslog(LOG_INFO, "Unlocked: %s", path);
    }
}

#ifndef BACKUP_H
#define BACKUP_H

#define ARCHIVE_COMMAND "tar -zcf"
#define BACKUP_FILE "$(date '+%FT%T').tar.gz"

void backup();

#endif


#ifndef FILE_LOCK_H
#define FILE_LOCK_H

#include <stdio.h>
#include <sys/file.h>


FILE *read_lock (char *);

FILE *write_lock (char *);

int unlock (FILE *);

int close_lock(FILE *);

#endif





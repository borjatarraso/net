/*
 * This file is a part of net Project
 *
 * alarm_thread.c - Asynchronous alarm using POSIX threads
 * Copyright (C) 2013  Borja Tarraso <borja.tarraso@member.fsf.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <pthread.h>

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
# define DPRINTF(arg) printf arg
#else
# define DPRINTF(arg)
#endif

#define err_abort(code,text) do { \
    fprintf(stderr, "%s at \"%s\":%d: %s\n", \
            text, __FILE__, __LINE__, strerror(code)); \
    abort(); \
    } while (0)
#define errno_abort(text) do { \
    fprintf(stderr, "%s at \"%s\":%d: %s\n", \
            text, __FILE__, __LINE__, strerror(errno)); \
    abort(); \
    } while (0)

typedef struct alarm_tag {
    int seconds;
    char message[64];
} alarm_t;

void* alarm_thread(void *arg)
{
    alarm_t *alarm = (alarm_t *)arg;
    int status = pthread_detach(pthread_self());

    if (status != 0)
        err_abort(status, "Detach thread");
    sleep(alarm->seconds);
    printf("(%d) %s\n", alarm->seconds, alarm->message);
    free(alarm);

    return NULL;
}

int main(int argc, char *argv[])
{
    int status;
    char line[128];
    alarm_t *alarm;
    pthread_t thread;

    while (1) {
        printf("Alarm> ");
        if (fgets(line, sizeof(line), stdin) == NULL)
            exit(0);
        if (strlen(line) <= 1)
            continue;
        alarm = (alarm_t *)malloc(sizeof(alarm_t));
        if(alarm == NULL)
            errno_abort("Allocate alarm");

        /* Parse input line into seconds and message up to 64 chars */
        if (sscanf(line, "%d %64[^\n]", &alarm->seconds, alarm->message) < 2) {
            fprintf(stderr, "Bad command\n");
            free(alarm);
        } else {
            status = pthread_create(&thread, NULL, alarm_thread, alarm);
            if (status != 0)
                err_abort(status, "Create alarm thread");
        }
    }

    return 0;
};

/*
 * This file is a part of net Project
 *
 * alarm_fork.c - Asynchronous alarm using fork()
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int status;
    char line[128];
    int seconds;
    pid_t pid;
    char message[64];

    while (1) {
        printf("Alarm>");
        if (fgets(line, sizeof(line), stdin) == NULL)
            exit(0);
        if (strlen(line) <= 1)
            continue;

        /* Parse input line into seconds and message up to 64 chars */
        if (sscanf(line, "%d %64[^\n]", &seconds, message) < 2)
            fprintf(stderr, "Bad command\n");
        else {
            pid = fork();
            if (pid == (pid_t)-1)
                exit(1);
            if (pid == (pid_t)0) {
                /* Child process: wait and print the message */
                sleep(seconds);
                printf("(%d) %s\n", seconds, message);
                exit(0);
            } else {
                /* Parent process: call waitpid() to collect the children which
                   already are terminated */
                do {
                    pid = waitpid((pid_t)-1, NULL, WNOHANG);
                    if (pid == (pid_t)-1)
                        exit(1);
                } while (pid != (pid_t)0);
            }
        }
    }

    return 0;
}

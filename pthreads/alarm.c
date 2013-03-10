/*
 * This file is a part of net Project
 *
 * alarm.c - Synchronous alarm
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int seconds;
    char line[128];
    char message[64];

    while (1) {
        printf("Alarm>" );
        if (fgets(line, sizeof(line), stdin) == NULL)
            exit(0);
        if (strlen(line) <= 1)
            continue;

        /* Parse input line into seconds and message up to 64 chars */
        if (sscanf(line, "%d %64[^\n]", &seconds, message) < 2) {
            fprintf(stderr, "Bad command\n");
        } else {
            sleep(seconds);
            printf("(%d) %s\n", seconds, message);
        }
    }

    return 0;
}

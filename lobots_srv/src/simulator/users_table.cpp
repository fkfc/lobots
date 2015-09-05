/***************************************************************************
 *   Copyright (C) 2009 by Felipe Kermentz Ferraz Costa   *
 *   felipekfcosta@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "users_table.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



void add_user (t_tab_users* table, int socket) {
	//strcpy(table->id[table->qty], id);
	table->socket[table->qty] = socket;
	table->qty++;
}

void remove_user (t_tab_users* table, int socket) {
	int i, j;
	i = 0;
	while ((i < table->qty) && (table->socket[i] != socket)) i++;
	if (i < table->qty) {
		for (j = table->qty - 1; j > i; j--) {
			strcpy(table->id[j-1], table->id[j]);
			table->socket[j-1] = table->socket[j];
		}
		table->qty--;
	}
}

char* get_id (t_tab_users *table, int socket) {
	int i = 0;
	while (i < table->qty && table->socket[i] != socket) i++;
	if (i < table->qty) return table->id[i];
}

int get_socket(t_tab_users *table, char* id) {
	int i = 0;
	while (i < table->qty && strcmp(table->id[i], id) != 0) i++;
	if (i < table->qty) return table->socket[i]; else return 0;
}

void set_id (t_tab_users* table, int socket, char* id) {
	int i = 0;
	while (i < table->qty && table->socket[i] != socket) i++;
	if (i < table->qty) strcpy(table->id[i], id);
}

t_tab_users* start_table() {
	t_tab_users* novo;
	novo = (t_tab_users*) malloc(sizeof(t_tab_users));
	novo->qty = 0;
	novo->iterator = 0;
	return novo;
}

char* get_next_id(t_tab_users* table) {
	if ((table->qty < 1) || (table->iterator >= table->qty)) {
	table->iterator = 0;
	return NULL;
	}
	return table->id[table->iterator++];
}

void reset_iterator(t_tab_users* table) {
	table->iterator = 0;
}

int iterator_finished(t_tab_users* table) {
	if (table->iterator >= table->qty) return 1;
	return 0;
}

void print_table(t_tab_users* table) {
	printf("--Connected clients--\n");
	reset_iterator(table);
	while (! iterator_finished(table)) {
		printf("%s\n", get_next_id(table));
	}
	printf("---end of list---\n");
}

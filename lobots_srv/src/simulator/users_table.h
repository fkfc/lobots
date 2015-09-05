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
 
#ifndef users_table_h
#define users_table_h

// same as SIM_MAX_ROBOS in simulador.h!
#define MAX_USERS 50 

// TODO: this is begging for a object-oriented recode

typedef struct {
	int qty;
	char id[MAX_USERS][150]; //robot name
	int socket[MAX_USERS];
	int iterator;
} t_tab_users;

void add_user (t_tab_users* table, int socket);
void remove_user (t_tab_users* table, int socket);
char* get_id (t_tab_users *table, int socket);
int get_socket(t_tab_users *table, char* id);
void set_id (t_tab_users* table, int socket, char* id);
t_tab_users* start_table();
char* get_next_id(t_tab_users* table);
void reset_iterator(t_tab_users* table);
int iterator_finished(t_tab_users* table);
void print_table(t_tab_users* table);


#endif

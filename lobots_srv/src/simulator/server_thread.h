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

#ifndef server_thread_H
#define server_thread_H

#include "simulador.h"

typedef struct { // thread arguments
	int port; // incoming connections port, eg 5555
	TSimulador *sim;
} tsrv_thread_args, *ptsrv_thread_args;

void *srv_thread_function(void *arg);
void StartRobot(TRobo *Robot);
void StopRobot(TRobo *Robot);
void SendWaypoint(TRobo *Robot, int x, int y, int z);


#endif
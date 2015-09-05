/***************************************************************************
 *   Copyright (C) 2009 by Felipe Kermentz Ferraz Costa                    *
 *   felipekfcosta@gmail.com                                               *
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


#include "trobot.h"
#include <stdio.h>


void robotfunc(void* arg) {
  TRobotClient *Robot = (TRobotClient*) arg;
  
  Robot->data.turn_angle = 1; // turns
  
  //prints the laser array
  printf("\n["); for (int i = 0; i < SIM_NUM_LASERS; i++) printf(" %.2f ", Robot->data.laser[i]); printf("]");
}

int main(int argc, char *argv[])
{
	// connect to a server
	// TRobotClient(char *robot_name, void(void*) robot_loop_procedure, char* server, int port)
	TRobotClient *mytest = new TRobotClient("example2", robotfunc, "127.0.0.1", 7123);
	for (;;); // lock the program: the robot function will now run on a separate thread
	return 0;
}

 

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

#include "protocol.h"
#include <string.h>
#include "reverse_endianness.h"

//check if package starts with "lp1"
bool IsPackageValid (char *package, int nbytes) {
	if (nbytes > 3 && 
		package[0] == 'l' &&
		package[1] == 'p' &&
		package[2] == '1') return true;
	else return false;
}

bool NewRobot(char* package, char* name) {
	if (package[3] == 'N') {
		int namelen;
		memcpy(&namelen, &package[4], sizeof(int));
		if (isBigEndian()) namelen = ReverseInt(namelen);
		memcpy(name, &package[4 + sizeof(int)], namelen);
		return true;
	} else return false;
}

bool GetUpdate(char *package, int nbytes, TRobo *Robot) {
	if (!IsPackageValid(package, nbytes)) return false;
	if (nbytes < 6 || package[3] != 'U') return false;
	if (package[4] == 'S' && package[5] == 'A') { // new speed and turn angle
		memcpy(&Robot->velocidade, &package[6], sizeof(float));
		if (isBigEndian()) Robot->velocidade = ReverseFloat(Robot->velocidade);
		memcpy(&Robot->angulo, &package[6 + sizeof(float)], sizeof(float));
		if (isBigEndian()) Robot->angulo = ReverseFloat(Robot->angulo);
		return true;
	} else if (package[4] == 'S' && package[5] == '0') { // new speed oly
		memcpy(&Robot->velocidade, &package[6], sizeof(float));
		if (isBigEndian()) Robot->velocidade = ReverseFloat(Robot->velocidade);
		return true;
	} else if (package[4] == '0' && package[5] == 'A') { // new turn angle only
		memcpy(&Robot->angulo, &package[6], sizeof(float));
		if (isBigEndian()) Robot->angulo = ReverseFloat(Robot->angulo);
		return true;
	} else if (package[4] == 'T' && package[5] == '0') { // teleport (new coordinates)
		int p = 6;
		int x, y, z;
		memcpy(&x, &package[p], sizeof(int)); p += sizeof(int);
		memcpy(&y, &package[p], sizeof(int)); p += sizeof(int);
		memcpy(&z, &package[p], sizeof(int));
		if (isBigEndian()) {
		  x = ReverseInt(x);
		  y = ReverseInt(y);
		  z = ReverseInt(z);
		}
		Robot->Teleport(x, y, z);
	        return true;
	} else if (package[4] == '0' && package[5] == '0') { // no updates
		return true;
	} else return false; // error reading package
}


void CreateUpdate(char *package, int *nbytes, TRobo *Robot) {	
	package[0] = 'l';
	package[1] = 'p';
	package[2] = '1';
	package[3] = 'U';
	
	*nbytes = 4;
	float odometro = isBigEndian()? ReverseFloat(Robot->odometro) : Robot->odometro;
	float bussola = isBigEndian()? ReverseFloat(Robot->bussola) : Robot->bussola;
	float velocidade = isBigEndian()? ReverseFloat(Robot->velocidade) : Robot->velocidade;
	float angulo = isBigEndian()? ReverseFloat(Robot->angulo) : Robot->angulo;
	
	memcpy(&package[*nbytes], &odometro, sizeof(float));
	*nbytes += sizeof(float);
	memcpy(&package[*nbytes], &bussola, sizeof(float));
	*nbytes += sizeof(float);
	memcpy(&package[*nbytes], &velocidade, sizeof(float));
	*nbytes += sizeof(float);
	memcpy(&package[*nbytes], &angulo, sizeof(float));
	*nbytes += sizeof(float);
	
	for (int j = 0; j < SIM_NUM_LASERS; j++) {
		float l = isBigEndian()? ReverseFloat(Robot->laser[j]) : Robot->laser[j];
		memcpy(&package[*nbytes], &l, sizeof(float));
		*nbytes += sizeof(float);
	}
	
	int x = isBigEndian()? ReverseInt(Robot->x) : Robot->x;
	int y = isBigEndian()? ReverseInt(Robot->y) : Robot->y;
	int z = isBigEndian()? ReverseInt(Robot->z) : Robot->z;
	
	memcpy(&package[*nbytes], &x, sizeof(int));
	*nbytes += sizeof(int);
	memcpy(&package[*nbytes], &y, sizeof(int));
	*nbytes += sizeof(int);
	memcpy(&package[*nbytes], &z, sizeof(int));
	*nbytes += sizeof(int);
	
	
	
}

void CreateWaypoint(char *package, int *nbytes, int x, int y, int z) {
	package[0] = 'l';
	package[1] = 'p';
	package[2] = '1';
	package[3] = 'W';
	
	if (isBigEndian()) {
	  x = ReverseInt(x);
	  y = ReverseInt(y);
	  z = ReverseInt(z);
	}
	
	*nbytes = 4;
	memcpy(&package[*nbytes], &x, sizeof(int));
	*nbytes += sizeof(int);
	memcpy(&package[*nbytes], &y, sizeof(int));
	*nbytes += sizeof(int);
	memcpy(&package[*nbytes], &z, sizeof(int));
	*nbytes += sizeof(int);
}

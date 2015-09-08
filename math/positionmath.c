/*
 * PositionMath.c
 *
 *  Created on: 21.4.2014
 *      Author: Samba
 */

#include "../common.h"
#include "math/positionmath.h"

/**
 * Compute bearing and elevation between current position and home
 */
void haversine_m(void)
{
	double R = 6371009; // m
	double dLat = (telem.homelat-telem.lat);
	double dLon = (telem.homelon-telem.lon);
	double lat1 = telem.lat;
	double lat2 = telem.homelat;

	double a = sin(dLat/2) * sin(dLat/2) +
	        sin(dLon/2) * sin(dLon/2) * cos(lat1) * cos(lat2);
	double c = 2 * atan2(sqrt(a), sqrt(1-a));
	double d = R * c;

	double y = sin(dLon) * cos(lat2);
	double x = cos(lat1)*sin(lat2) -
	        sin(lat1)*cos(lat2)*cos(dLon);
	double brng = RAD2DEG(atan2(y, x));
	telem.homedistance=d;
	telem.homebearing=brng;
}

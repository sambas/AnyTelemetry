/*
 * common.h
 *
 * Created: 30.3.2014 13:46:56
 *  Author: Samba
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#include "ch.h"
#include "hal.h"
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#define NAZA
#define RIDGE
//#define D8R


// Conversion macro
#define RAD2DEG(rad)            ((rad) * (180.0f / M_PI))
#define DEG2RAD(deg)            ((deg) * (M_PI / 180.0f))

// Status1
#define ARMED 1
#define BATT_LEVEL1 2
#define BATT_LEVEL2 3
// Status2
#define ARMED2 1
// Status3
#define HOMESET 1

//Flight mode
#define MANUAL 0
#define GPS 1
#define FAILSAFE 2
#define ATTI 3

typedef struct TELEM{
	volatile uint16_t   battery; // mV
	volatile uint16_t   bec; // mV
	volatile float		baroaltitude; // m
	volatile double homelat; // rad
	volatile double homelon; // rad
	volatile float galt; // m
	volatile double lat;	// rad
	volatile double lon;	// rad
	volatile int sats;
	volatile float gpsspeed; // from ned m/s
	volatile float X;
	volatile float Y;
	volatile float Z;
	volatile float Roll;	// deg
	volatile float Pitch;	// deg
	volatile float Yaw;		// deg
	volatile float vN; // m/s
	volatile float vE; // m/s
	volatile float vD; // m/s
	volatile double homedistance; // m
	volatile double homebearing; // deg
	volatile uint8_t status1;
	volatile uint8_t status2;
	volatile uint8_t status3;
	volatile uint8_t fmode;
	volatile int16_t input[8];
	volatile int16_t th_raw;
	volatile int16_t roll_raw;
	volatile int16_t pitch_raw;
	volatile int16_t yaw_raw;

	volatile uint16_t  MwSensorPresent;
	volatile uint32_t  MwSensorActive;
	volatile uint8_t MwVersion;
	volatile uint8_t MwVBat;
	volatile int16_t MwVario;
	volatile uint8_t armed;
	volatile uint8_t previousarmedstatus;  // for statistics after disarming
	volatile uint16_t armedangle;           // for capturing direction at arming
	volatile uint16_t GPS_distanceToHome;
	volatile uint8_t GPS_fix;
	volatile int32_t GPS_latitude;
	volatile int32_t GPS_longitude;
	volatile int16_t GPS_altitude;
	volatile uint16_t GPS_speed;
	volatile uint16_t old_GPS_speed;
	volatile int16_t GPS_directionToHome;
	volatile uint8_t GPS_numSat;
	volatile uint8_t GPS_waypoint_step;
	//volatile uint16_t I2CError=0;
	//volatile uint16_t cycleTime=0;
	volatile uint16_t pMeterSum;	//mAh
	volatile uint16_t MwRssi;
	volatile uint32_t GPS_time;        //local time of coord calc - haydent

	volatile uint16_t cycleTime;
	volatile uint16_t I2CError;

	volatile uint16_t  MwAccSmooth[3];       // Those will hold Accelerator data
	volatile int32_t  MwAltitude;                         // This hold barometric value
	volatile int32_t  old_MwAltitude;                     // This hold barometric value


	volatile int MwAngle[2];           // Those will hold Accelerator Angle
	volatile uint16_t MwRcData[8];   // This hold receiver pulse signal
	volatile int16_t MwHeading;
	volatile uint16_t MWAmperage; // 1/100 A
	volatile uint16_t debug[4];
} TELEMETRY;

extern TELEMETRY telem;

// Core runtime settings
typedef struct core_t {
    uint8_t mainport;
    uint8_t telemport;
} core_t;

extern core_t core;


#endif /* COMMON_H_ */

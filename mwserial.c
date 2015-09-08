#include "ch.h"
#include "hal.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "board.h"
//#include "mw.h"
#include "mwserial.h"

//#include "cli.h"
//#include "telemetry_common.h"

#define VERSION 230
extern TELEMETRY telem;


enum {
    BOXARM = 0,
    BOXANGLE,
    BOXHORIZON,
    BOXBARO,
    BOXVARIO,
    BOXMAG,
    BOXHEADFREE,
    BOXHEADADJ,
    BOXCAMSTAB,
    BOXCAMTRIG,
    BOXGPSHOME,
    BOXGPSHOLD,
    BOXPASSTHRU,
    BOXBEEPERON,
    BOXLEDMAX,
    BOXLEDLOW,
    BOXLLIGHTS,
    BOXCALIB,
    BOXGOV,
    BOXOSD,
    BOXTELEMETRY,
    CHECKBOXITEMS
};




// Multiwii Serial Protocol 0
#define MSP_VERSION              0
#define CAP_PLATFORM_32BIT          ((uint32_t)1 << 31)
#define CAP_DYNBALANCE              ((uint32_t)1 << 2)
#define CAP_FLAPS                   ((uint32_t)1 << 3)

#define MSP_IDENT                100    //out message         multitype + multiwii version + protocol version + capability variable
#define MSP_STATUS               101    //out message         cycletime & errors_count & sensor present & box activation & current setting number
#define MSP_RAW_IMU              102    //out message         9 DOF
#define MSP_SERVO                103    //out message         8 servos
#define MSP_MOTOR                104    //out message         8 motors
#define MSP_RC                   105    //out message         8 rc chan and more
#define MSP_RAW_GPS              106    //out message         fix, numsat, lat, lon, alt, speed, ground course
#define MSP_COMP_GPS             107    //out message         distance home, direction home
#define MSP_ATTITUDE             108    //out message         2 angles 1 heading
#define MSP_ALTITUDE             109    //out message         altitude, variometer
#define MSP_ANALOG               110    //out message         vbat, powermetersum, rssi if available on RX
#define MSP_RC_TUNING            111    //out message         rc rate, rc expo, rollpitch rate, yaw rate, dyn throttle PID
#define MSP_PID                  112    //out message         P I D coeff (9 are used currently)
#define MSP_BOX                  113    //out message         BOX setup (number is dependant of your setup)
#define MSP_MISC                 114    //out message         powermeter trig
#define MSP_MOTOR_PINS           115    //out message         which pins are in use for motors & servos, for GUI
#define MSP_BOXNAMES             116    //out message         the aux switch names
#define MSP_PIDNAMES             117    //out message         the PID names
#define MSP_WP                   118    //out message         get a WP, WP# is in the payload, returns (WP#, lat, lon, alt, flags) WP#0-home, WP#16-poshold
#define MSP_BOXIDS               119    //out message         get the permanent IDs associated to BOXes
#define MSP_SERVO_CONF           120    //out message         Servo settings
#define MSP_NAV_STATUS           121    //out message         Returns navigation status
#define MSP_NAV_CONFIG           122    //out message         Returns navigation parameters

#define MSP_SET_RAW_RC           200    //in message          8 rc chan
#define MSP_SET_RAW_GPS          201    //in message          fix, numsat, lat, lon, alt, speed
#define MSP_SET_PID              202    //in message          P I D coeff (9 are used currently)
#define MSP_SET_BOX              203    //in message          BOX setup (number is dependant of your setup)
#define MSP_SET_RC_TUNING        204    //in message          rc rate, rc expo, rollpitch rate, yaw rate, dyn throttle PID
#define MSP_ACC_CALIBRATION      205    //in message          no param
#define MSP_MAG_CALIBRATION      206    //in message          no param
#define MSP_SET_MISC             207    //in message          powermeter trig + 8 free for future use
#define MSP_RESET_CONF           208    //in message          no param
#define MSP_SET_WP               209    //in message          sets a given WP (WP#,lat, lon, alt, flags)
#define MSP_SELECT_SETTING       210    //in message          Select Setting Number (0-2)
#define MSP_SET_HEAD             211    //in message          define a new heading hold direction
#define MSP_SET_SERVO_CONF       212    //in message          Servo settings
#define MSP_SET_MOTOR            214    //in message          PropBalance function
#define MSP_SET_NAV_CONFIG       215    //in message          Sets nav config parameters - write to the eeprom

// #define MSP_BIND                 240    //in message          no param

#define MSP_EEPROM_WRITE         250    //in message          no param

#define MSP_DEBUGMSG             253    //out message         debug string buffer
#define MSP_DEBUG                254    //out message         debug1,debug2,debug3,debug4

// Additional commands that are not compatible with MultiWii
#define MSP_UID                  160    //out message         Unique device ID
#define MSP_ACC_TRIM             240    //out message         get acc angle trim values
#define MSP_SET_ACC_TRIM         239    //in message          set acc angle trim values
#define MSP_GPSSVINFO            164    //out message         get Signal Strength (only U-Blox)

#define INBUF_SIZE 64

struct box_t {
    const uint8_t boxIndex;         // this is from boxnames enum
    const char *boxName;            // GUI-readable box name
    const uint8_t permanentId;      //
} boxes[] = {
    { BOXARM, "ARM;", 0 },
    { BOXANGLE, "ANGLE;", 1 },
    { BOXHORIZON, "HORIZON;", 2 },
    { BOXBARO, "BARO;", 3 },
    { BOXVARIO, "VARIO;", 4 },
    { BOXMAG, "MAG;", 5 },
    { BOXHEADFREE, "HEADFREE;", 6 },
    { BOXHEADADJ, "HEADADJ;", 7 },
    { BOXCAMSTAB, "CAMSTAB;", 8 },
    { BOXCAMTRIG, "CAMTRIG;", 9 },
    { BOXGPSHOME, "GPS HOME;", 10 },
    { BOXGPSHOLD, "GPS HOLD;", 11 },
    { BOXPASSTHRU, "PASSTHRU;", 12 },
    { BOXBEEPERON, "BEEPER;", 13 },
    { BOXLEDMAX, "LEDMAX;", 14 },
    { BOXLEDLOW, "LEDLOW;", 15 },
    { BOXLLIGHTS, "LLIGHTS;", 16 },
    { BOXCALIB, "CALIB;", 17 },
    { BOXGOV, "GOVERNOR;", 18 },
    { BOXOSD, "OSD SW;", 19 },
    { BOXTELEMETRY, "TELEMETRY;", 20 },
    { CHECKBOXITEMS, 0, 0xFF }
};

// this is calculated at startup based on enabled features.
uint8_t availableBoxes[CHECKBOXITEMS];
// this is the number of filled indexes in above array
uint8_t numberBoxItems = 0;
// from mixer.c
int16_t motor_disarmed[8];

static const char pidnames[] =
    "ROLL;"
    "PITCH;"
    "YAW;"
    "ALT;"
    "Pos;"
    "PosR;"
    "NavR;"
    "LEVEL;"
    "MAG;"
    "VEL;";

static uint8_t checksum, indRX, inBuf[INBUF_SIZE];
static uint8_t cmdMSP;

void serialWrite(uint8_t port, uint8_t data)
{
	(void)port;
	sdPut(&SD1,data);
}

void serialize32(uint32_t a)
{
    static uint8_t t;
    t = a;
    serialWrite(core.mainport, t);
    checksum ^= t;
    t = a >> 8;
    serialWrite(core.mainport, t);
    checksum ^= t;
    t = a >> 16;
    serialWrite(core.mainport, t);
    checksum ^= t;
    t = a >> 24;
    serialWrite(core.mainport, t);
    checksum ^= t;
}

void serialize16(int16_t a)
{
    static uint8_t t;
    t = a;
    serialWrite(core.mainport, t);
    checksum ^= t;
    t = a >> 8 & 0xff;
    serialWrite(core.mainport, t);
    checksum ^= t;
}

void serialize8(uint8_t a)
{
    serialWrite(core.mainport, a);
    checksum ^= a;
}

/*uint8_t read8(void)
{
    return inBuf[indRX++] & 0xff;
}

uint16_t read16(void)
{
    uint16_t t = read8();
    t += (uint16_t) read8() << 8;
    return t;
}

uint32_t read32(void)
{
    uint32_t t = read16();
    t += (uint32_t) read16() << 16;
    return t;
}*/

void headSerialResponse(uint8_t err, uint8_t s)
{
    serialize8('$');
    serialize8('M');
    serialize8(err ? '!' : '>');
    checksum = 0;               // start calculating a new checksum
    serialize8(s);
    serialize8(cmdMSP);
}

void headSerialReply(uint8_t s)
{
    headSerialResponse(0, s);
}

void headSerialError(uint8_t s)
{
    headSerialResponse(1, s);
}

void tailSerialReply(void)
{
    serialize8(checksum);
}

void s_struct(uint8_t *cb, uint8_t siz)
{
    headSerialReply(siz);
    while (siz--)
        serialize8(*cb++);
}

void serializeNames(const char *s)
{
    const char *c;
    for (c = s; *c; c++)
        serialize8(*c);
}

void serializeBoxNamesReply(void)
{
    int i, idx, j, flag = 1, count = 0, len;

reset:
    // in first run of the loop, we grab total size of junk to be sent
    // then come back and actually send it
    for (i = 0; i < numberBoxItems; i++) {
        idx = availableBoxes[i];
        len = strlen(boxes[idx].boxName);
        if (flag) {
            count += len;
        } else {
            for (j = 0; j < len; j++)
                serialize8(boxes[idx].boxName[j]);
        }
    }

    if (flag) {
        headSerialReply(count);
        flag = 0;
        goto reset;
    }
}

void serialInit(uint32_t baudrate)
{
	(void)baudrate;
    int idx;

    //core.mainport = uartOpen(USART1, NULL, baudrate, MODE_RXTX);

    // calculate used boxes based on features and fill availableBoxes[] array
    memset(availableBoxes, 0xFF, sizeof(availableBoxes));

    idx = 0;
    availableBoxes[idx++] = BOXARM;
    if (1) {
        availableBoxes[idx++] = BOXANGLE;
        availableBoxes[idx++] = BOXHORIZON;
    }
    if (1) {
        availableBoxes[idx++] = BOXBARO;
        if (1)
            availableBoxes[idx++] = BOXVARIO;
    }
    if (1) {
        availableBoxes[idx++] = BOXMAG;
        availableBoxes[idx++] = BOXHEADFREE;
        availableBoxes[idx++] = BOXHEADADJ;
    }
    if (0)
        availableBoxes[idx++] = BOXCAMSTAB;
    if (1) {
        availableBoxes[idx++] = BOXGPSHOME;
        availableBoxes[idx++] = BOXGPSHOLD;
    }
    if (0)
        availableBoxes[idx++] = BOXPASSTHRU;
    availableBoxes[idx++] = BOXBEEPERON;
    if (0)
        availableBoxes[idx++] = BOXCALIB;
    availableBoxes[idx++] = BOXOSD;
    if (0)
        availableBoxes[idx++] = BOXTELEMETRY;
    numberBoxItems = idx;
}

static void evaluateCommand(void)
{
    uint32_t i, tmp, junk;
    uint8_t wp_no;
    int32_t lat = 0, lon = 0, alt = 0;

    switch (cmdMSP) {
    /*case MSP_SET_RAW_RC:
        for (i = 0; i < 8; i++)
            rcData[i] = read16();
        headSerialReply(0);
        break;
    case MSP_SET_ACC_TRIM:
        cfg.angleTrim[PITCH] = read16();
        cfg.angleTrim[ROLL]  = read16();
        headSerialReply(0);
        break;
    case MSP_SET_RAW_GPS:
        f.GPS_FIX = read8();
        GPS_numSat = read8();
        GPS_coord[LAT] = read32();
        GPS_coord[LON] = read32();
        GPS_altitude = read16();
        GPS_speed = read16();
        GPS_update |= 2;        // New data signalisation to GPS functions
        headSerialReply(0);
        break;
    case MSP_SET_PID:
        for (i = 0; i < PIDITEMS; i++) {
            cfg.P8[i] = read8();
            cfg.I8[i] = read8();
            cfg.D8[i] = read8();
        }
        headSerialReply(0);
        break;
    case MSP_SET_BOX:
        for (i = 0; i < numberBoxItems; i++)
            cfg.activate[availableBoxes[i]] = read16();
        headSerialReply(0);
        break;
    case MSP_SET_RC_TUNING:
        cfg.rcRate8 = read8();
        cfg.rcExpo8 = read8();
        cfg.rollPitchRate = read8();
        cfg.yawRate = read8();
        cfg.dynThrPID = read8();
        cfg.thrMid8 = read8();
        cfg.thrExpo8 = read8();
        headSerialReply(0);
        break;
    case MSP_SET_MISC:
        read16(); // powerfailmeter
        mcfg.minthrottle = read16();
        mcfg.maxthrottle = read16();
        mcfg.mincommand = read16();
        cfg.failsafe_throttle = read16();
        read16();
        read32();
        cfg.mag_declination = read16() * 10;
        mcfg.vbatscale = read8();           // actual vbatscale as intended
        mcfg.vbatmincellvoltage = read8();  // vbatlevel_warn1 in MWC2.3 GUI
        mcfg.vbatmaxcellvoltage = read8();  // vbatlevel_warn2 in MWC2.3 GUI
        read8();                            // vbatlevel_crit (unused)
        headSerialReply(0);
        break;
    case MSP_SET_MOTOR:
        for (i = 0; i < 8; i++)
            motor_disarmed[i] = read16();
        headSerialReply(0);
        break;
    case MSP_SELECT_SETTING:
        if (!f.ARMED) {
            mcfg.current_profile = read8();
            if (mcfg.current_profile > 2)
                mcfg.current_profile = 0;
            // this writes new profile index and re-reads it
            writeEEPROM(0, false);
        }
        headSerialReply(0);
        break;
    case MSP_SET_HEAD:
        magHold = read16();
        headSerialReply(0);
        break;*/
    case MSP_IDENT:
        headSerialReply(7);
        serialize8(VERSION);                // multiwii version
        serialize8(3); // type of multicopter
//        serialize8(mcfg.mixerConfiguration); // type of multicopter
        serialize8(MSP_VERSION);            // MultiWii Serial Protocol Version
        //serialize32(CAP_PLATFORM_32BIT | CAP_DYNBALANCE | (mcfg.flaps_speed ? CAP_FLAPS : 0));        // "capability"
        serialize32(CAP_PLATFORM_32BIT | CAP_DYNBALANCE | 0);        // "capability"
        break;
    case MSP_STATUS:
        headSerialReply(11);
        //serialize16(cycleTime);
        //serialize16(i2cGetErrorCounter());
#ifdef NAZA
        serialize16(100);
        serialize16(0);
        //serialize16(sensors(SENSOR_ACC) | sensors(SENSOR_BARO) << 1 | sensors(SENSOR_MAG) << 2 | sensors(SENSOR_GPS) << 3 | sensors(SENSOR_SONAR) << 4);
        serialize16(1 | 1 << 1 | 1 << 2 | 1 << 3 | 0 << 4);
        // OK, so you waste all the fucking time to have BOXNAMES and BOXINDEXES etc, and then you go ahead and serialize enabled shit simply by stuffing all
        // the bits in order, instead of setting the enabled bits based on BOXINDEX. WHERE IS THE FUCKING LOGIC IN THIS, FUCKWADS.
        // Serialize the boxes in the order we delivered them, until multiwii retards fix their shit
        junk = 0;
        tmp = ((telem.fmode == ATTI)?1:0) << BOXANGLE | 0 << BOXHORIZON |
                    0 << BOXBARO | 0 << BOXMAG | 0 << BOXHEADFREE | 0 << BOXHEADADJ |
                    0 << BOXCAMSTAB | 0 << BOXCAMTRIG |
                    ((telem.fmode == FAILSAFE)?1:0) << BOXGPSHOME | ((telem.fmode == GPS)?1:0) << BOXGPSHOLD |
                    0 << BOXPASSTHRU |
                    0 << BOXBEEPERON |
                    0 << BOXLEDMAX |
                    0 << BOXLLIGHTS |
                    0 << BOXVARIO |
                    0 << BOXCALIB |
                    0 << BOXGOV |
                    0 << BOXOSD |
                    0 << BOXTELEMETRY |
                    ((telem.status1 & 0x01)?1:0)  << BOXARM;
        for (i = 0; i < numberBoxItems; i++) {
            int flag = (tmp & (1 << availableBoxes[i]));
            if (flag)
                junk |= 1 << i;
        }
        serialize32(junk);
        //serialize8(mcfg.current_profile);
        serialize8(1);
#else
        serialize16(telem.cycleTime);
        serialize16(telem.I2CError);
        serialize16(telem.MwSensorPresent);
        serialize32(telem.MwSensorActive);
        serialize8(1);
#endif
        break;
    case MSP_RAW_IMU:
        headSerialReply(18);
        // Retarded hack until multiwiidorks start using real units for sensor data
        /*if (acc_1G > 1024) {
            for (i = 0; i < 3; i++)
                serialize16(accSmooth[i] / 8);
        } else {
            for (i = 0; i < 3; i++)
                serialize16(accSmooth[i]);
        }
        for (i = 0; i < 3; i++)
            serialize16(gyroData[i]);
        for (i = 0; i < 3; i++)
            serialize16(magADC[i]);*/
        for(i=0;i<9;i++)
        {
        	serialize16(0);
        }
        break;
    case MSP_SERVO:
        //s_struct((uint8_t *)&servo, 16);
        break;
    case MSP_SERVO_CONF:
        /*headSerialReply(56);
        for (i = 0; i < MAX_SERVOS; i++) {
            serialize16(cfg.servoConf[i].min);
            serialize16(cfg.servoConf[i].max);
            serialize16(cfg.servoConf[i].middle);
            serialize8(cfg.servoConf[i].rate);
        }*/
        break;
    case MSP_SET_SERVO_CONF:
        /*headSerialReply(0);
        for (i = 0; i < MAX_SERVOS; i++) {
            cfg.servoConf[i].min = read16();
            cfg.servoConf[i].max = read16();
            cfg.servoConf[i].middle = read16();
            cfg.servoConf[i].rate = read8();
        }*/
        break;
    case MSP_MOTOR:
        //s_struct((uint8_t *)motor, 16);
        break;
    case MSP_RC:
        headSerialReply(16);
        for (i = 0; i < 8; i++)
            serialize16(telem.input[i]);
        break;
    case MSP_RAW_GPS:
        headSerialReply(16);
        /*serialize8(f.GPS_FIX);
        serialize8(GPS_numSat);
        serialize32(GPS_coord[LAT]);
        serialize32(GPS_coord[LON]);
        serialize16(GPS_altitude);
        serialize16(GPS_speed);
        serialize16(GPS_ground_course);*/
#ifdef NAZA
        serialize8(	(telem.sats > 5)?1:0 );
#else
        serialize8(telem.GPS_fix);
#endif
        serialize8(telem.sats);
        serialize32((int32_t)(RAD2DEG(telem.lat)*10e6));
        serialize32((int32_t)(RAD2DEG(telem.lon)*10e6));
        serialize16((int16_t)telem.galt);
        serialize16((uint16_t)(telem.gpsspeed*100));
#ifdef NAZA
        serialize16((int16_t)telem.Yaw*10);
#else
        serialize16((int16_t)telem.MwHeading*10);
#endif
        break;
    case MSP_COMP_GPS:
        headSerialReply(5);
#ifdef NAZA
        serialize16((uint16_t)telem.homedistance);
        serialize16((uint16_t)(telem.homebearing+360)%360);
#else
        serialize16((uint16_t)telem.GPS_distanceToHome);
        serialize16((uint16_t)telem.GPS_directionToHome);
#endif
        serialize8(1);
        break;
    case MSP_ATTITUDE:
        headSerialReply(6);
        serialize16((int16_t)(telem.Roll*10));
        serialize16((int16_t)(telem.Pitch*-10));
        serialize16((int16_t)telem.Yaw);
        break;
    case MSP_ALTITUDE:
        headSerialReply(6);
        serialize32((int32_t)(telem.baroaltitude*100));
        serialize16((int16_t)(telem.vD*-100));
        break;
    case MSP_ANALOG:
        headSerialReply(7);
        serialize8((uint8_t)(telem.battery/100));
        serialize16((uint16_t)(telem.pMeterSum)); // power meter trash
        serialize16(1023);
        serialize16((uint16_t)(telem.MWAmperage*10)); // amperage
        break;
    case MSP_RC_TUNING:
        /*headSerialReply(7);
        serialize8(cfg.rcRate8);
        serialize8(cfg.rcExpo8);
        serialize8(cfg.rollPitchRate);
        serialize8(cfg.yawRate);
        serialize8(cfg.dynThrPID);
        serialize8(cfg.thrMid8);
        serialize8(cfg.thrExpo8);*/
        break;
    case MSP_PID:
        /*headSerialReply(3 * PIDITEMS);
        for (i = 0; i < PIDITEMS; i++) {
            serialize8(cfg.P8[i]);
            serialize8(cfg.I8[i]);
            serialize8(cfg.D8[i]);
        }*/
        break;
    case MSP_PIDNAMES:
        /*headSerialReply(sizeof(pidnames) - 1);
        serializeNames(pidnames);*/
        break;
    case MSP_BOX:
        /*headSerialReply(2 * numberBoxItems);
        for (i = 0; i < numberBoxItems; i++)
            serialize16(cfg.activate[availableBoxes[i]]);*/
        break;
    case MSP_BOXNAMES:
        // headSerialReply(sizeof(boxnames) - 1);
        serializeBoxNamesReply();
        break;
    case MSP_BOXIDS:
        headSerialReply(numberBoxItems);
        for (i = 0; i < numberBoxItems; i++)
            serialize8(availableBoxes[i]);
        break;
    case MSP_MISC:
        /*headSerialReply(2 * 6 + 4 + 2 + 4);
        serialize16(0); // intPowerTrigger1 (aka useless trash)
        serialize16(mcfg.minthrottle);
        serialize16(mcfg.maxthrottle);
        serialize16(mcfg.mincommand);
        serialize16(cfg.failsafe_throttle);
        serialize16(0); // plog useless shit
        serialize32(0); // plog useless shit
        serialize16(cfg.mag_declination / 10); // TODO check this shit
        serialize8(mcfg.vbatscale);
        serialize8(mcfg.vbatmincellvoltage);
        serialize8(mcfg.vbatmaxcellvoltage);
        serialize8(0);*/
        break;
    case MSP_MOTOR_PINS:
        /*headSerialReply(8);
        for (i = 0; i < 8; i++)
            serialize8(i + 1);*/
        break;
    case MSP_WP:
        /*wp_no = read8();    // get the wp number
        headSerialReply(18);
        if (wp_no == 0) {
            lat = GPS_home[LAT];
            lon = GPS_home[LON];
        } else if (wp_no == 16) {
            lat = GPS_hold[LAT];
            lon = GPS_hold[LON];
        }
        serialize8(wp_no);
        serialize32(lat);
        serialize32(lon);
        serialize32(AltHold);           // altitude (cm) will come here -- temporary implementation to test feature with apps
        serialize16(0);                 // heading  will come here (deg)
        serialize16(0);                 // time to stay (ms) will come here
        serialize8(0);                  // nav flag will come here
        */
        break;
    case MSP_SET_WP:
        /*wp_no = read8();    //get the wp number
        lat = read32();
        lon = read32();
        alt = read32();     // to set altitude (cm)
        read16();           // future: to set heading (deg)
        read16();           // future: to set time to stay (ms)
        read8();            // future: to set nav flag
        if (wp_no == 0) {
            GPS_home[LAT] = lat;
            GPS_home[LON] = lon;
            f.GPS_HOME_MODE = 0;        // with this flag, GPS_set_next_wp will be called in the next loop -- OK with SERIAL GPS / OK with I2C GPS
            f.GPS_FIX_HOME = 1;
            if (alt != 0)
                AltHold = alt;          // temporary implementation to test feature with apps
        } else if (wp_no == 16) {       // OK with SERIAL GPS  --  NOK for I2C GPS / needs more code dev in order to inject GPS coord inside I2C GPS
            GPS_hold[LAT] = lat;
            GPS_hold[LON] = lon;
            if (alt != 0)
                AltHold = alt;          // temporary implementation to test feature with apps
            nav_mode = NAV_MODE_WP;
            GPS_set_next_wp(&GPS_hold[LAT], &GPS_hold[LON]);
        }
        headSerialReply(0);*/
        break;
    case MSP_RESET_CONF:
        /*if (!f.ARMED)
            checkFirstTime(true);
        headSerialReply(0);*/
        break;
    case MSP_ACC_CALIBRATION:
        /*if (!f.ARMED)
            calibratingA = CALIBRATING_ACC_CYCLES;
        headSerialReply(0);*/
        break;
    case MSP_MAG_CALIBRATION:
        /*if (!f.ARMED)
            f.CALIBRATE_MAG = 1;
        headSerialReply(0);*/
        break;
    case MSP_EEPROM_WRITE:
        /*if (f.ARMED) {
            headSerialError(0);
        } else {
            writeEEPROM(0, true);
            headSerialReply(0);
        }*/
        break;
    case MSP_DEBUG:
        /*headSerialReply(8);
        // make use of this crap, output some useful QA statistics
        debug[3] = ((hse_value / 1000000) * 1000) + (SystemCoreClock / 1000000);         // XX0YY [crystal clock : core clock]
        for (i = 0; i < 4; i++)
            serialize16(debug[i]);      // 4 variables are here for general monitoring purpose
            */
        break;

    // Additional commands that are not compatible with MultiWii
    case MSP_ACC_TRIM:
        /*headSerialReply(4);
        serialize16(cfg.angleTrim[PITCH]);
        serialize16(cfg.angleTrim[ROLL]);*/
        break;
    case MSP_UID:
        /*headSerialReply(12);
        serialize32(U_ID_0);
        serialize32(U_ID_1);
        serialize32(U_ID_2);*/
        break;
    case MSP_GPSSVINFO:
        /*headSerialReply(1 + (GPS_numCh * 4));
        serialize8(GPS_numCh);
           for (i = 0; i < GPS_numCh; i++){
               serialize8(GPS_svinfo_chn[i]);
               serialize8(GPS_svinfo_svid[i]);
               serialize8(GPS_svinfo_quality[i]);
               serialize8(GPS_svinfo_cno[i]);
            }*/
        break;
    default:                   // we do not know how to handle the (valid) message, indicate error MSP $M!
        headSerialError(0);
        break;
    }
    tailSerialReply();
}

// evaluate all other incoming serial data
static void evaluateOtherData(uint8_t sr)
{
	(void)sr;
    //if (sr == '#')
    //    cliProcess();
    //else if (sr == mcfg.reboot_character)
    //    systemReset(true);      // reboot to bootloader
}

int serialTotalBytesWaiting(uint8_t port)
{
	(void)port;
	return chQSpaceI(&SD1.iqueue);
}

uint8_t serialRead(uint8_t port)
{
	(void)port;
	uint8_t buf;
    sdReadTimeout(&SD1, &buf, 1, MS2ST(0));
    return buf;
}


void serialCom(void)
{
    uint8_t c;
    static uint8_t offset;
    static uint8_t dataSize;
    static enum _serial_state {
        IDLE,
        HEADER_START,
        HEADER_M,
        HEADER_ARROW,
        HEADER_SIZE,
        HEADER_CMD,
    } c_state = IDLE;

    // in cli mode, all serial stuff goes to here. enter cli mode by sending #
    if (0) {
        //cliProcess();
        return;
    }

    while (serialTotalBytesWaiting(core.mainport)) {
        c = serialRead(core.mainport);

        if (c_state == IDLE) {
            c_state = (c == '$') ? HEADER_START : IDLE;
            if (c_state == IDLE)
                evaluateOtherData(c); // if not armed evaluate all other incoming serial data
        } else if (c_state == HEADER_START) {
            c_state = (c == 'M') ? HEADER_M : IDLE;
        } else if (c_state == HEADER_M) {
            c_state = (c == '<') ? HEADER_ARROW : IDLE;
        } else if (c_state == HEADER_ARROW) {
            if (c > INBUF_SIZE) {       // now we are expecting the payload size
                c_state = IDLE;
                continue;
            }
            dataSize = c;
            offset = 0;
            checksum = 0;
            indRX = 0;
            checksum ^= c;
            c_state = HEADER_SIZE;      // the command is to follow
        } else if (c_state == HEADER_SIZE) {
            cmdMSP = c;
            checksum ^= c;
            c_state = HEADER_CMD;
        } else if (c_state == HEADER_CMD && offset < dataSize) {
            checksum ^= c;
            inBuf[offset++] = c;
        } else if (c_state == HEADER_CMD && offset >= dataSize) {
            if (checksum == c) {        // compare calculated and transferred checksum
                evaluateCommand();      // we got a valid packet, evaluate it
            }
            c_state = IDLE;
        }
    }
}

static WORKING_AREA(waSerial1_Thread, 1024);
static msg_t Serial1_Thread(void *p) {
  EventListener s1EventListener;

  (void)p;
  serialInit(115200);
  chEvtRegisterMask(chnGetEventSource(&SD1), &s1EventListener, CHN_INPUT_AVAILABLE);
  while (TRUE) {
	flagsmask_t flags;

    chEvtWaitOne(CHN_INPUT_AVAILABLE);
    flags = chEvtGetAndClearFlags(&s1EventListener);
    if (flags & CHN_INPUT_AVAILABLE) {
      /* Data available read here.*/
    	serialCom();
    }
    if (flags & CHN_OUTPUT_EMPTY) {
      /* Data sent, you may transmit from here.*/
    }
    if (flags & (SD_PARITY_ERROR | SD_FRAMING_ERROR | SD_OVERRUN_ERROR |
                 SD_NOISE_ERROR | SD_BREAK_DETECTED)) {
      /* Some receive error happened.*/
    }
  }
  return 0;
}

static const SerialConfig sd1_config =
{
  115200,
  0,
  USART_CR2_STOP1_BITS | USART_CR2_LINEN,
  0
};


void initMwSerial(void)
{
	  /*
	   * Activates the serial driver 1 using the default configuration, pins
	   * are pre-configured in board.h.
	   */
	  sdStart(&SD1, &sd1_config);

	  chThdCreateStatic(waSerial1_Thread, sizeof(waSerial1_Thread), LOWPRIO, Serial1_Thread, NULL);
}


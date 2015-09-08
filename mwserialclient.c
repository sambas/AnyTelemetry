#include "ch.h"
#include "hal.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "board.h"
//#include "mw.h"
#include "mwserialclient.h"

//#include "cli.h"
//#include "telemetry_common.h"

#define VERSION 230


// ---------------------------------------------------------------------------------------
// Defines imported from Multiwii Serial Protocol MultiWii_shared svn r1337
#define MSP_VERSION              0

//to multiwii developpers/committers : do not add new MSP messages without a proper argumentation/agreement on the forum
#define MSP_IDENT                100   //out message         multitype + multiwii version + protocol version + capability variable
#define MSP_STATUS               101   //out message         cycletime & errors_count & sensor present & box activation & current setting number
#define MSP_RAW_IMU              102   //out message         9 DOF
#define MSP_SERVO                103   //out message         8 servos
#define MSP_MOTOR                104   //out message         8 motors
#define MSP_RC                   105   //out message         8 rc chan and more
#define MSP_RAW_GPS              106   //out message         fix, numsat, lat, lon, alt, speed, ground course
#define MSP_COMP_GPS             107   //out message         distance home, direction home
#define MSP_ATTITUDE             108   //out message         2 angles 1 heading
#define MSP_ALTITUDE             109   //out message         altitude, variometer
#define MSP_ANALOG               110   //out message         vbat, powermetersum, rssi if available on RX
#define MSP_RC_TUNING            111   //out message         rc rate, rc expo, rollpitch rate, yaw rate, dyn throttle PID
#define MSP_PID                  112   //out message         P I D coeff (9 are used currently)
#define MSP_BOX                  113   //out message         BOX setup (number is dependant of your setup)
#define MSP_MISC                 114   //out message         powermeter trig
#define MSP_MOTOR_PINS           115   //out message         which pins are in use for motors & servos, for GUI
#define MSP_BOXNAMES             116   //out message         the aux switch names
#define MSP_PIDNAMES             117   //out message         the PID names
#define MSP_BOXIDS               119   //out message         get the permanent IDs associated to BOXes
#define MSP_NAV_STATUS           121   //out message	     Returns navigation status

#define MSP_CELLS                130   //out message         FrSky SPort Telemtry

#define MSP_SET_RAW_RC           200   //in message          8 rc chan
#define MSP_SET_RAW_GPS          201   //in message          fix, numsat, lat, lon, alt, speed
#define MSP_SET_PID              202   //in message          P I D coeff (9 are used currently)
#define MSP_SET_BOX              203   //in message          BOX setup (number is dependant of your setup)
#define MSP_SET_RC_TUNING        204   //in message          rc rate, rc expo, rollpitch rate, yaw rate, dyn throttle PID
#define MSP_ACC_CALIBRATION      205   //in message          no param
#define MSP_MAG_CALIBRATION      206   //in message          no param
#define MSP_SET_MISC             207   //in message          powermeter trig + 8 free for future use
#define MSP_RESET_CONF           208   //in message          no param
#define MSP_SET_WP               209   //in message          sets a given WP (WP#,lat, lon, alt, flags)
#define MSP_SELECT_SETTING       210   //in message          Select Setting Number (0-2)
#define MSP_SET_HEAD             211   //in message          define a new heading hold direction

#define MSP_BIND                 240   //in message          no param

#define MSP_EEPROM_WRITE         250   //in message          no param

#define MSP_DEBUGMSG             253   //out message         debug string buffer
#define MSP_DEBUG                254   //out message         debug1,debug2,debug3,debug4
// End of imported defines from Multiwii Serial Protocol MultiWii_shared svn r1333
// ---------------------------------------------------------------------------------------

// Private MSP for use with the GUI
#define MSP_OSD                  220   //in message          starts epprom send to OSD GUI
// Subcommands
#define OSD_NULL                 0
#define OSD_READ_CMD             1
#define OSD_WRITE_CMD            2
#define OSD_GET_FONT             3
#define OSD_SERIAL_SPEED         4
#define OSD_RESET                5
#define OSD_DEFAULT              6
#define OSD_SENSORS              7
// End private MSP for use with the GUI



#define REQ_MSP_IDENT     (1 <<  0)
#define REQ_MSP_STATUS    (1 <<  1)
#define REQ_MSP_RAW_IMU   (1 <<  2)
#define REQ_MSP_RC        (1 <<  3)
#define REQ_MSP_RAW_GPS   (1 <<  4)
#define REQ_MSP_COMP_GPS  (1 <<  5)
#define REQ_MSP_ATTITUDE  (1 <<  6)
#define REQ_MSP_ALTITUDE  (1 <<  7)
#define REQ_MSP_ANALOG    (1 <<  8)
#define REQ_MSP_RC_TUNING (1 <<  9)
#define REQ_MSP_PID       (1 << 10)
#define REQ_MSP_BOX       (1 << 11)
#define REQ_MSP_FONT      (1 << 12)
#define REQ_MSP_DEBUG     (1 << 13)
#define REQ_MSP_CELLS     (1 << 14)
#define REQ_MSP_NAV_STATUS  32768 //(1 << 15)



#define hi_speed_cycle  50
#define lo_speed_cycle  100


// May be moved in GlobalVariables.h
unsigned long previous_millis_low=0;
unsigned long previous_millis_high =0;
//----------------

uint32_t modeMSPRequests;
uint32_t queuedMSPRequests;


// Mode bits
struct {
  uint8_t armed;
  uint8_t stable;
  uint8_t horizon;
  uint8_t baro;
  uint8_t mag;
  uint16_t camstab;
  uint16_t gpshome;
  uint16_t gpshold;
  uint16_t passthru;
  uint32_t osd_switch;
  uint32_t llights;
  uint32_t gpsmission;
  uint32_t gpsland;
}mode;


extern TELEMETRY telem;



#define SERIALBUFFERSIZE 200
static uint8_t serialBuffer[SERIALBUFFERSIZE]; // this hold the imcoming string from serial O string
static uint8_t receiverIndex;
static uint8_t dataSize;
static uint8_t cmdMSP;
static uint8_t rcvChecksum;
static uint8_t readIndex;

extern uint32_t read32(void);
extern uint16_t read16(void);
extern uint8_t read8(void);
extern uint8_t Serialread(void);
extern int Serialavailable(void);
extern void Serialwrite(uint8_t data);


extern void setMspRequests(void);
extern void serialMSPCheck(void);
extern void serialMSPreceive(void);
extern void headSerialRequest(void);


uint32_t read32() {
  uint32_t t = (uint32_t)read16();
  t |= (uint32_t)read16()<<16;
  return t;
}

uint16_t read16() {
  uint16_t t = (uint16_t)read8();
  t |= (uint16_t)read8()<<8;
  return t;
}

uint8_t read8()  {
  return serialBuffer[readIndex++];
}

// --------------------------------------------------------------------------------------
// Here are decoded received commands from MultiWii
void serialMSPCheck()
{
  readIndex = 0;

  if (cmdMSP==MSP_IDENT)
  {
    telem.MwVersion= read8();                             // MultiWii Firmware version
    modeMSPRequests &=~ REQ_MSP_IDENT;
  }

  if (cmdMSP==MSP_STATUS)
  {
	  telem.cycleTime=read16();
	  telem.I2CError=read16();
    telem.MwSensorPresent = read16();
    telem.MwSensorActive = read32();
    telem.armed = (telem.MwSensorActive & mode.armed) != 0;
    read8();
  }

  if (cmdMSP==MSP_RAW_IMU)
  {
	  uint8_t i;
    for(i=0;i<3;i++)
    	telem.MwAccSmooth[i] = read16();
  }
  if (cmdMSP==MSP_RC)
  {
	  uint8_t i;
    for(i=0;i<8;i++)
    	telem.input[i] = read16();
  }

  if (cmdMSP==MSP_RAW_GPS)
  {
	  telem.GPS_fix=read8();
	  telem.sats=read8();
	  telem.lat = DEG2RAD(((int32_t)read32())/10.0e6);
	  telem.lon = DEG2RAD(((int32_t)read32())/10.0e6);
	  telem.galt = (int16_t)read16();

#if defined I2CGPS_SPEED
	  telem.GPS_speed = read16()*10;
//    gpsfix(); untested
#else
    telem.gpsspeed = read16()/100.0;
#endif
    telem.MwHeading = ((int16_t)read16())/10.0;
  }

  if (cmdMSP==MSP_COMP_GPS)
  {
	  telem.GPS_distanceToHome=read16();
#ifdef I2CGPS_DISTANCE
    gpsdistancefix();
#endif

    telem.GPS_directionToHome=read16();
    read8(); //missing
#ifdef GPSTIME
    GPS_time = read32();        //local time of coord calc - haydent
#endif
  }

  if (cmdMSP==MSP_NAV_STATUS)
  {
     read8();
     read8();
     read8();
     telem.GPS_waypoint_step=read8();
     read8();
     read8();
     read8();
  }

  if (cmdMSP==MSP_ATTITUDE)
  {
    telem.Roll = ((int16_t)read16())/10.0f;
    telem.Pitch = ((int16_t)read16())/-10.0f;
    telem.Yaw = ((int16_t)read16());
#ifdef HEADINGCORRECT
    if (telem.MwHeading >= + 180) telem.MwHeading -= 360;
#endif
  }

  if (cmdMSP==MSP_DEBUG)
  {
	  uint8_t i;
    for(i=0;i<3;i++)
      telem.debug[i] = read16();
  }

  if (cmdMSP==MSP_ALTITUDE)
  {
	  telem.baroaltitude = ((int32_t)read32())/100.0f;
	  telem.vD = ((int16_t)read16())/-100.0f;
  }

  if (cmdMSP==MSP_ANALOG)
  {
	  telem.battery=read8()*100;
	  telem.pMeterSum=read16();
	  telem.MwRssi = read16();
	  telem.MWAmperage = read16();
 #ifdef AMPERAGECORRECT
	  telem.MWAmperage = telem.MWAmperage * 10;
#endif
 }

#ifdef BOXNAMES
  if(cmdMSP==MSP_BOXNAMES) {
    uint32_t bit = 1;
    uint8_t remaining = dataSize;
    uint8_t len = 0;
    char firstc, lastc;

    mode.armed = 0;
    mode.stable = 0;
    mode.baro = 0;
    mode.mag = 0;
    mode.gpshome = 0;
    mode.gpshold = 0;
    mode.llights = 0;
    mode.camstab = 0;
    mode.osd_switch = 0;

    while(remaining > 0) {
      char c = read8();
      if(len == 0)
        firstc = c;
      len++;
      if(c == ';') {
        // Found end of name; set bit if first and last c matches.
        if(firstc == 'A') {
          if(lastc == 'M') // "ARM;"
            mode.armed |= bit;
          if(lastc == 'E') // "ANGLE;"
            mode.stable |= bit;
        }
        if(firstc == 'H' && lastc == 'N') // "HORIZON;"
          mode.horizon |= bit;
        if(firstc == 'M' && lastc == 'G') // "MAG;"
           mode.mag |= bit;
        if(firstc == 'B' && lastc == 'O') // "BARO;"
          mode.baro |= bit;
        if(firstc == 'L' && lastc == 'S') // "LLIGHTS;"
          mode.llights |= bit;
        if(firstc == 'C' && lastc == 'B') // "CAMSTAB;"
          mode.camstab |= bit;
        if(firstc == 'G') {
          if(lastc == 'E') // "GPS HOME;"
            mode.gpshome |= bit;
          if(lastc == 'D') // "GPS HOLD;"
            mode.gpshold |= bit;
        }
        if(firstc == 'P' && lastc == 'U')  mode.passthru |= bit; // "Passthru;"
        if(firstc == 'O' && lastc == 'W') // "OSD SW;"
          mode.osd_switch |= bit;

        len = 0;
        bit <<= 1L;
      }
      lastc = c;
      --remaining;
    }
    modeMSPRequests &=~ REQ_MSP_BOX;
  }
#else
  if(cmdMSP==MSP_BOXIDS) {
    uint32_t bit = 1;
    uint8_t remaining = dataSize;

    mode.armed = 0;
    mode.stable = 0;
    mode.horizon = 0;
    mode.baro = 0;
    mode.mag = 0;
    mode.gpshome = 0;
    mode.gpshold = 0;
    mode.gpsmission = 0;
    mode.gpsland = 0;
    mode.llights = 0;
    mode.passthru = 0;
    mode.osd_switch = 0;
    mode.camstab = 0;

    while(remaining > 0) {
      char c = read8();
      switch(c) {
      case 0:
        mode.armed |= bit;
        break;
      case 1:
        mode.stable |= bit;
        break;
      case 2:
        mode.horizon |= bit;
        break;
      case 3:
        mode.baro |= bit;
        break;
      case 5:
        mode.mag |= bit;
        break;
      case 8:
        mode.camstab |= bit;
        break;
      case 10:
        mode.gpshome |= bit;
        break;
      case 11:
        mode.gpshold |= bit;
        break;
      case 12:
        mode.passthru  |= bit;
        break;
      case 16:
        mode.llights |= bit;
        break;
      case 19:
        mode.osd_switch |= bit;
        break;
      case 20:
        mode.gpsmission |= bit;
        break;
      case 21:
        mode.gpsland |= bit;
        break;
      }
      bit <<= 1;
      --remaining;
    }
    modeMSPRequests &=~ REQ_MSP_BOX;
  }
#endif
}
// End of decoded received commands from MultiWii
// --------------------------------------------------------------------------------------

void serialMSPreceive()
{
  uint8_t c;

  static enum _serial_state {
    IDLE,
    HEADER_START,
    HEADER_M,
    HEADER_ARROW,
    HEADER_SIZE,
    HEADER_CMD,
  }
  c_state = IDLE;

  while(Serialavailable())
  {
    c = Serialread();

    if (c_state == IDLE)
    {
      c_state = (c=='$') ? HEADER_START : IDLE;
    }
    else if (c_state == HEADER_START)
    {
      c_state = (c=='M') ? HEADER_M : IDLE;
    }
    else if (c_state == HEADER_M)
    {
      c_state = (c=='>') ? HEADER_ARROW : IDLE;
    }
    else if (c_state == HEADER_ARROW)
    {
      if (c > SERIALBUFFERSIZE)
      {  // now we are expecting the payload size
        c_state = IDLE;
      }
      else
      {
        dataSize = c;
        c_state = HEADER_SIZE;
        rcvChecksum = c;
      }
    }
    else if (c_state == HEADER_SIZE)
    {
      c_state = HEADER_CMD;
      cmdMSP = c;
      rcvChecksum ^= c;
      receiverIndex=0;
    }
    else if (c_state == HEADER_CMD)
    {
      rcvChecksum ^= c;
      if(receiverIndex == dataSize) // received checksum byte
      {
        if(rcvChecksum == 0) {
            serialMSPCheck();
        }
        c_state = IDLE;
      }
      else
        serialBuffer[receiverIndex++]=c;
    }
  }
}

void blankserialRequest(uint8_t requestMSP)
{
  headSerialRequest();
  Serialwrite((uint8_t)0x00);
  Serialwrite(requestMSP);
  Serialwrite(requestMSP);
}

void headSerialRequest (void) {
  Serialwrite('$');
  Serialwrite('M');
  Serialwrite('<');
}

void Serialwrite(uint8_t data)
{
	sdPut(&SD2,data);
}

int Serialavailable()
{
	return chQSpaceI(&SD2.iqueue);
}

uint8_t Serialread()
{
	uint8_t buf;
    sdReadTimeout(&SD2, &buf, 1, MS2ST(0));
    return buf;
}

/*
 * Transmitter thread.
 */
static WORKING_AREA(waMwSerialClientSend_Thread, 256);
static msg_t MwSerialClientSend_Thread(void * p) {
  (void)p;
  chRegSetThreadName("transmitter");

  setMspRequests();
  blankserialRequest(MSP_IDENT);
  systime_t time = chTimeNow();     // T0

  while (!chThdShouldTerminate()) {
	  time += MS2ST(10);            // Next deadline

	  // Blink Basic Sanity Test Led at 1hz
	  /*if(timer.tenthSec>10)
	    digitalWrite(LEDPIN,HIGH);
	  else
	    digitalWrite(LEDPIN,LOW);
*/
	  //---------------  Start Timed Service Routines  ---------------------------------------
	  unsigned long currentMillis = chTimeNow();

	  if((currentMillis - previous_millis_low) >= lo_speed_cycle)  // 10 Hz (Executed every 100ms)
	  {
	    previous_millis_low = previous_millis_low+lo_speed_cycle;
	    blankserialRequest(MSP_ATTITUDE);

	  }  // End of slow Timed Service Routine (100ms loop)

	  if((currentMillis - previous_millis_high) >= hi_speed_cycle)  // 20 Hz (Executed every 50ms)
	  {
	    previous_millis_high = previous_millis_high+hi_speed_cycle;

	      uint8_t MSPcmdsend=0;
	      if(queuedMSPRequests == 0)
	        queuedMSPRequests = modeMSPRequests;
	      uint32_t req = queuedMSPRequests & -queuedMSPRequests;
	      queuedMSPRequests &= ~req;
	      switch(req) {
	      case REQ_MSP_IDENT:
	       MSPcmdsend = MSP_IDENT;
	        break;
	      case REQ_MSP_STATUS:
	        MSPcmdsend = MSP_STATUS;
	        break;
	      case REQ_MSP_RAW_IMU:
	        MSPcmdsend = MSP_RAW_IMU;
	        break;
	      case REQ_MSP_RC:
	        MSPcmdsend = MSP_RC;
	        break;
	      case REQ_MSP_RAW_GPS:
	        MSPcmdsend = MSP_RAW_GPS;
	        break;
	      case REQ_MSP_COMP_GPS:
	        MSPcmdsend = MSP_COMP_GPS;
	        break;
	      case REQ_MSP_ATTITUDE:
	        MSPcmdsend = MSP_ATTITUDE;
	        break;
	      case REQ_MSP_ALTITUDE:
	        MSPcmdsend = MSP_ALTITUDE;
	        break;
	      case REQ_MSP_ANALOG:
	        MSPcmdsend = MSP_ANALOG;
	        break;
	      case REQ_MSP_RC_TUNING:
	        MSPcmdsend = MSP_RC_TUNING;
	        break;
	      case REQ_MSP_PID:
	        MSPcmdsend = MSP_PID;
	        break;
	      case REQ_MSP_BOX:
	#ifdef USE_BOXNAMES
	        MSPcmdsend = MSP_BOXNAMES;
	#else
	        MSPcmdsend = MSP_BOXIDS;
	#endif
	         break;
	      case REQ_MSP_FONT:
	         MSPcmdsend = MSP_OSD;
	         break;
	      case REQ_MSP_DEBUG:
	         MSPcmdsend = MSP_DEBUG;
	         break;
	#if defined SPORT
	      case REQ_MSP_CELLS:
	         MSPcmdsend = MSP_CELLS;
	         break;
	#endif
	      case REQ_MSP_NAV_STATUS:
	//           if(MwSensorActive&mode.gpsmission)
	         MSPcmdsend = MSP_NAV_STATUS;
	      break;
	    }
	      blankserialRequest(MSPcmdsend);
	  }  // End of fast Timed Service Routine (50ms loop)

	  chThdSleepUntil(time);
  }
  return 0;
}


static WORKING_AREA(waMwSerialClient_Thread, 1024);
static msg_t MwSerialClient_Thread(void *p) {
  EventListener s2EventListener;

  (void)p;
  chEvtRegisterMask(chnGetEventSource(&SD2), &s2EventListener, CHN_INPUT_AVAILABLE);
  while (TRUE) {
	flagsmask_t flags;

    chEvtWaitOne(CHN_INPUT_AVAILABLE);
    flags = chEvtGetAndClearFlags(&s2EventListener);
    if (flags & CHN_INPUT_AVAILABLE) {
      /* Data available read here.*/
    	serialMSPreceive();
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

void setMspRequests() {
    modeMSPRequests =
      REQ_MSP_IDENT|
      REQ_MSP_STATUS|
      REQ_MSP_RAW_GPS|
      REQ_MSP_COMP_GPS|
      REQ_MSP_ATTITUDE|
      REQ_MSP_DEBUG|
      REQ_MSP_ALTITUDE|REQ_MSP_RAW_IMU;

      modeMSPRequests |= REQ_MSP_RC;

    if(mode.armed == 0)
      modeMSPRequests |= REQ_MSP_BOX;

  modeMSPRequests |= REQ_MSP_ANALOG;


  // so we do not send requests that are not needed.
  queuedMSPRequests &= modeMSPRequests;
}



static const SerialConfig sd2_config =
{
  115200,
  0,
  USART_CR2_STOP1_BITS | USART_CR2_LINEN,
  0
};


void initMwSerialClient(void)
{
	  /*
	   * Activates the serial driver 1 using the default configuration, pins
	   * are pre-configured in board.h.
	   */
	  sdStart(&SD2, &sd2_config);




	  chThdCreateStatic(waMwSerialClient_Thread, sizeof(waMwSerialClient_Thread), LOWPRIO, MwSerialClient_Thread, NULL);

	  chThdCreateStatic(waMwSerialClientSend_Thread, sizeof(waMwSerialClientSend_Thread), LOWPRIO, MwSerialClientSend_Thread, NULL);
}



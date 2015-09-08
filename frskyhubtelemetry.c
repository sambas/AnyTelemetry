/*
 * frskyhubtelemetry.c
 *
 * Created: 30.3.2014 13:46:56
 *  Author: Samba
 */

#include "common.h"
#include "frskyhubtelemetry.h"

#define CYCLETIME             125

/*
datasources to ids

ID_ACC_X telem.X
ID_ACC_Y telem.Y
ID_ACC_Z telem.Z
ID_ALTITUDE telem.baroaltitude
ID_GPS_SPEED telem.gpsspeed
ID_GPS_ALTIDUTE telem.galt
ID_COURSE telem.Yaw
ID_TEMPRATURE1 telem.sats
ID_TEMPRATURE2 telem.th_raw
ID_COURSE_BP	telem.MWHeading
 */

// from sensors.c
extern uint8_t batteryCellCount;


void serialWrite2(uint8_t port, uint8_t data)
{
	(void)port;
	//sdPut(&SD2,data);
	sdPut(&SD1,data);
}


static void sendDataHead(uint8_t id)
{
    serialWrite2(core.telemport, PROTOCOL_HEADER);
    serialWrite2(core.telemport, id);
}

static void sendTelemetryTail(void)
{
    serialWrite2(core.telemport, PROTOCOL_TAIL);
}

static void serializeFrsky(uint8_t data)
{
    // take care of byte stuffing
    if (data == 0x5e) {
        serialWrite2(core.telemport, 0x5d);
        serialWrite2(core.telemport, 0x3e);
    } else if (data == 0x5d) {
        serialWrite2(core.telemport, 0x5d);
        serialWrite2(core.telemport, 0x3d);
    } else
        serialWrite2(core.telemport, data);
}

static void serialize16(int16_t a)
{
    uint8_t t;
    t = a;
    serializeFrsky(t);
    t = a >> 8 & 0xff;
    serializeFrsky(t);
}

static void sendAccel(void)
{
    sendDataHead(ID_ACC_X);
	serialize16(telem.Roll * 1000);
    sendDataHead(ID_ACC_Y);
    serialize16(telem.Pitch * 1000);
    sendDataHead(ID_ACC_Z);
    serialize16(telem.Yaw * 10);
}

static void sendBaro(void)
{
    sendDataHead(ID_ALTITUDE_BP);
    serialize16(telem.baroaltitude);
    sendDataHead(ID_ALTITUDE_AP);
    serialize16((int16_t)(telem.baroaltitude*100) % 100);
}

static void sendTime(void)
{
    uint32_t seconds = chTimeNow() / 1000;
    uint8_t minutes = (seconds / 60) % 60;

    // if we fly for more than an hour, something's wrong anyway
    sendDataHead(ID_HOUR_MINUTE);
    serialize16(minutes << 8);
    sendDataHead(ID_SECOND);
    serialize16(seconds % 60);
}

static void sendGPS(void)
{
	double lat = telem.lat*180.0f/M_PI;
	int32_t latdeg = abs((int32_t)lat);
	int32_t latmin = abs((lat-latdeg)*600000);
	int32_t latgps = latdeg*1000000+latmin;
	double lon = telem.lon*180.0f/M_PI;
	int32_t londeg = abs((int32_t)lon);
	int32_t lonmin = abs((lon-londeg)*600000);
	int32_t longps = londeg*1000000+lonmin;

    sendDataHead(ID_LATITUDE_BP);
    serialize16(abs(latgps) / 10000);
    sendDataHead(ID_LATITUDE_AP);
    serialize16(abs(latgps) % 10000);

    sendDataHead(ID_N_S);
    serialize16(lat < 0 ? 'S' : 'N');

    sendDataHead(ID_LONGITUDE_BP);
    serialize16(abs(longps) / 10000);
    sendDataHead(ID_LONGITUDE_AP);
    serialize16(abs(longps) % 10000);
    sendDataHead(ID_E_W);
    serialize16(lon < 0 ? 'W' : 'E');
}

static void sendGPSSpeed(void)
{
    sendDataHead(ID_GPS_SPEED_BP);
    serialize16((uint32_t)(telem.gpsspeed*1944)/1000);
    sendDataHead(ID_GPS_SPEED_AP);
    serialize16((uint32_t)(telem.gpsspeed*1944)%1000);
}

static void sendGPSAltitude(void)
{
    sendDataHead(ID_GPS_ALTIDUTE_BP);
    serialize16((uint32_t)(telem.galt));
    sendDataHead(ID_GPS_ALTIDUTE_AP);
    serialize16((uint32_t)(telem.galt*1000)%1000);
}
/*
 * Send voltage via ID_VOLT
 *
 * NOTE: This sends voltage divided by batteryCellCount. To get the real
 * battery voltage, you need to multiply the value by batteryCellCount.
 */
static void sendVoltage(void)
{
    static uint16_t currentCell = 0;
    uint16_t cellNumber;
    uint32_t cellVoltage;
    uint16_t payload;
    uint16_t batteryCellCount=3;
    /*
     * Note: Fuck the pdf. Format for Voltage Data for single cells is like this:
     *
     *  llll llll cccc hhhh
     *  l: Low voltage bits
     *  h: High voltage bits
     *  c: Cell number (starting at 0)
     */
    cellVoltage = telem.battery / batteryCellCount;

    // Map to 12 bit range
    cellVoltage = (cellVoltage * 21) / 42;

    cellNumber = currentCell % batteryCellCount;

    // Cell number is at bit 9-12
    payload = (cellNumber << 4);

    // Lower voltage bits are at bit 0-8
    payload |= ((cellVoltage & 0x0ff) << 8);

    // Higher voltage bits are at bits 13-15
    payload |= ((cellVoltage & 0xf00) >> 8);

    sendDataHead(ID_VOLT);
    serialize16(payload);

    currentCell++;
    currentCell %= batteryCellCount;
}

/*
 * Send voltage with ID_VOLTAGE_AMP
 */
static void sendVoltageAmp(void)
{
    uint16_t voltage = (telem.battery * 11) / 210;

    sendDataHead(ID_VOLTAGE_AMP_BP);
    serialize16(voltage / 100);
    sendDataHead(ID_VOLTAGE_AMP_AP);
    serialize16(((voltage % 100) + 5) / 10);
}

static void sendHeading(void)
{
    sendDataHead(ID_COURSE_BP);
    serialize16((uint16_t)telem.MwHeading);
    sendDataHead(ID_COURSE_AP);
    serialize16(0);

}

static void sendT1(void)
{
    sendDataHead(ID_TEMPRATURE1);
    serialize16((uint16_t)telem.sats);
}

static void sendT2(void)
{
    sendDataHead(ID_TEMPRATURE2);
    serialize16((uint16_t)telem.debug[2]);
}

static uint8_t cycleNum = 0;

void sendTelemetry(void)
{
    /*if (mcfg.telemetry_softserial == TELEMETRY_UART && !f.ARMED)
        return;

    if (serialTotalBytesWaiting(core.telemport) != 0)
        return;*/

        cycleNum++;

        // Sent every 125ms
        sendAccel();
        sendT1();
        sendT2();
        sendTelemetryTail();

        if ((cycleNum % 4) == 0) {      // Sent every 500ms
            sendBaro();
            sendHeading();
            sendTelemetryTail();
        }

        if ((cycleNum % 8) == 0) {      // Sent every 1s
            sendVoltage();
            sendVoltageAmp();
            sendGPS();
            sendGPSSpeed();
            sendGPSAltitude();
            sendTelemetryTail();
        }

        if (cycleNum == 40) {     //Frame 3: Sent every 5s
            cycleNum = 0;
            sendTime();
            sendTelemetryTail();
        }

}

static const SerialConfig sd2_config =
{
  9600,
  0,
  USART_CR2_STOP1_BITS | USART_CR2_LINEN|USART_CR2_TXINV|USART_CR2_RXINV,
  0
};

static WORKING_AREA(waSerial2_Thread, 1024);
static msg_t Serial2_Thread(void *p) {
	(void)p;
  systime_t time = chTimeNow();     // T0
  while (TRUE) {
    time += MS2ST(125);            // Next deadline
    sendTelemetry();
    chThdSleepUntil(time);
  }
  return 0;
}

/*void initFrskyHubTelemetry(void) {
  sdStart(&SD2, &sd2_config);

  chThdCreateStatic(waSerial2_Thread, sizeof(waSerial2_Thread), LOWPRIO, Serial2_Thread, NULL);
}*/

void initFrskyHubTelemetry(void) {
  /*
   * Activates the serial driver 2
   */
  sdStart(&SD1, &sd2_config);

  chThdCreateStatic(waSerial2_Thread, sizeof(waSerial2_Thread), LOWPRIO, Serial2_Thread, NULL);
}



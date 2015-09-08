/*
 * frskytelemetry.c
 *
 * Created: 30.3.2014 13:46:56
 *  Author: Samba
 */

#include "common.h"
#include "frskytelemetry.h"


volatile uint8_t lastRx;
uint8_t FR_ID_count = 0;
uint8_t cell_count = 0;
uint8_t latlong_flag = 0;
uint32_t latlong = 0;
uint8_t first=0;

static void frskytelemetry_send(uint16_t id, int32_t value) {
    palSetPad(GPIOB, GPIOB_LED1);
    uint8_t databuffer[8];
    uint8_t sendbuffer[16];
	uint16_t crc = 0;
	uint8_t i=0;
	databuffer[0]=DATA_FRAME;
	uint8_t *bytes = (uint8_t*)&id;
	databuffer[1]=bytes[0];
	databuffer[2]=bytes[1];
	bytes = (uint8_t*)&value;
	databuffer[3]=bytes[0];
	databuffer[4]=bytes[1];
	databuffer[5]=bytes[2];
	databuffer[6]=bytes[3];
	for(i=0; i<7; i++)
	{
		crc += databuffer[i];   //0-1FF
		crc += crc >> 8;   //0-100
		crc &= 0x00ff;
		crc += crc >> 8;   //0-0FF
		crc &= 0x00ff;
	}
	databuffer[7]=0xFF-crc;

	i=0;
	uint8_t j=0;
	for( j=0;j<8;j++)
	{
	    // take care of byte stuffing
		uint8_t data = databuffer[j];
	    if ((data == 0x7e) || (data == 0x7d)) {
	    	sendbuffer[i++]=0x7d;
	    	sendbuffer[i++]=data ^ STUFF_MASK;
	    } else {
	    	sendbuffer[i++]=data;
	    }
	}
	uartStartSend(&UARTD3, i, sendbuffer);
}

static void frskytelemetry_input(uint8_t c) {
	uint8_t data = c;

	if (lastRx == START_STOP && (data == 0xA1))
	{
		switch(FR_ID_count) {
			case 0:
				frskytelemetry_send(VFAS_FIRST_ID,telem.battery/10);
				break;
			case 1:
				frskytelemetry_send(ALT_FIRST_ID,(int32_t)(telem.baroaltitude*100));
				break;
			case 2:
#ifdef NAZA
				if(telem.sats > 5) // this should use gps fix status
#else
				if(telem.GPS_fix)
#endif
				{
					int32_t frlon = (int32_t)(telem.lon*180.0f/M_PI*600000.0f);
					if(frlon < 0)
						latlong=((labs(frlon)))  | 0xC0000000;
					else
						latlong=((labs(frlon)))  | 0x80000000;
					frskytelemetry_send(GPS_LONG_LATI_FIRST_ID,latlong);
				}
				break;
			case 3:
#ifdef NAZA
				if(telem.sats > 5) // this should use gps fix status
#else
				if(telem.GPS_fix)
#endif
				{
					int32_t frlat = (int32_t)(telem.lat*180.0f/M_PI*600000.0f);
					if(frlat < 0 )
						latlong=(labs(frlat)) | 0x40000000;
					else
						latlong=(labs(frlat));
					frskytelemetry_send(GPS_LONG_LATI_FIRST_ID,latlong);
				}
				break;
			case 4:
				frskytelemetry_send(T1_FIRST_ID,(int32_t)(telem.sats));
				break;
			case 5:
#ifdef NAZA
				frskytelemetry_send(T2_FIRST_ID,telem.fmode);	// NAZA flightmode
#else
				frskytelemetry_send(T2_FIRST_ID,telem.debug[2]); // CF_F4 GPS status
#endif
				break;
			case 6:
				frskytelemetry_send(ACCX_FIRST_ID,(int32_t)(telem.Roll*10));
				break;
			case 7:
				frskytelemetry_send(ACCY_FIRST_ID,(int32_t)(telem.Pitch*10));
				break;
			case 8:
				frskytelemetry_send(ACCZ_FIRST_ID,(int32_t)(telem.Yaw*10));
				break;
			case 9:
				frskytelemetry_send(GPS_COURS_FIRST_ID,(uint32_t)(telem.MwHeading));
				break;
			case 10:
				frskytelemetry_send(VARIO_FIRST_ID,((int32_t)(telem.vD*-100)));
				break;
			case 11:
				frskytelemetry_send(GPS_ALT_FIRST_ID,((int32_t)(telem.galt*100)));
				break;
			case 12:
				frskytelemetry_send(GPS_SPEED_FIRST_ID,(uint32_t)(telem.gpsspeed*1944)); // m/s to knot*1000
				//frskytelemetry_send(GPS_SPEED_FIRST_ID,(uint32_t)(telem.gpsspeed*1943.844)); // m/s to knot*1000
				break;
			case 13:
#ifdef NAZA
				frskytelemetry_send(FUEL_FIRST_ID,(uint32_t)(telem.homedistance)); // m
#else
				frskytelemetry_send(FUEL_FIRST_ID,(uint32_t)(telem.GPS_distanceToHome)); // m
#endif
				break;
			case 14:
				frskytelemetry_send(RPM_FIRST_ID,(uint32_t)(telem.sats*2)); // blades=2!
				break;
			case 15:
				frskytelemetry_send(CURR_FIRST_ID,(uint32_t)(telem.MWAmperage/10)); // 1/10A!
				break;
			case 16:
				frskytelemetry_send(AIR_SPEED_FIRST_ID,(uint32_t)(telem.debug[1]/10)); //dm/s
				break;
		}
		FR_ID_count++;
		if(FR_ID_count > MAX_ID_COUNT) FR_ID_count = 0;
	}
	lastRx=data;
}



/*
 * This callback is invoked when a transmission buffer has been completely
 * read by the driver.
 */
static void txend1(UARTDriver *uartp) {

  (void)uartp;
}

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend2(UARTDriver *uartp) {

  (void)uartp;
  chSysLockFromIsr();
  palClearPad(GPIOB, GPIOB_LED1);
  chSysUnlockFromIsr();
}

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void rxerr(UARTDriver *uartp, uartflags_t e) {

  (void)uartp;
  (void)e;
}

/*
 * This callback is invoked when a character is received but the application
 * was not ready to receive it, the character is passed as parameter.
 */
static void rxchar(UARTDriver *uartp, uint16_t c) {

  (void)uartp;
  (void)c;
  /* Flashing the LED each time a character is received.*/
  chSysLockFromIsr();
  frskytelemetry_input(c);
  chSysUnlockFromIsr();
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(UARTDriver *uartp) {

  (void)uartp;
}

/*
 * UART driver configuration structure.
 */
static UARTConfig uart_cfg_1 = {
  txend1,
  txend2,
  rxend,
  rxchar,
  rxerr,
  57600,
  0,
  USART_CR2_TXINV|USART_CR2_RXINV,
  USART_CR3_HDSEL
};

void initFrskyTelemetry(void) {
  /*
   * Activates the uart driver 3
   */
  uartStart(&UARTD3, &uart_cfg_1);
}


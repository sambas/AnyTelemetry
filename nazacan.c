/*
 * nazacan.c
 *
 *  Created on: 7.6.2014
 *      Author: Samba
 */

#include "nazacan.h"
#include "common.h"
#include "math/quaternion.h"
#include "math/positionmath.h"

#define CAN_BUFFER_SIZE 512 /**< Size of the circular receive buffer, must be power of 2 */
#define CAN_BUFFER_MASK ( CAN_BUFFER_SIZE - 1)

#if ( CAN_BUFFER_SIZE & CAN_BUFFER_MASK )
#error CAN buffer size is not a power of 2
#endif


static volatile uint8_t CAN_Buf[CAN_BUFFER_SIZE];
static volatile uint16_t CAN_RxHead=0;
static volatile uint16_t CAN_RxTail=0;
static volatile uint8_t CAN_LastRxError=0;

static volatile uint8_t CAN_Buf1[CAN_BUFFER_SIZE];
static volatile uint16_t CAN_RxHead1=0;
static volatile uint16_t CAN_RxTail1=0;
static volatile uint8_t CAN_LastRxError1=0;

volatile uint8_t    newdata108=0;
volatile uint8_t    newdata090=0;

extern TELEMETRY telem;
extern float q[4];

/*
** high byte error return code of can_getc()
*/
#define CAN_FRAME_ERROR 0x0800 /**< Framing Error by CAN */
#define CAN_OVERRUN_ERROR 0x0400 /**< Overrun condition by CAN */
#define CAN_BUFFER_OVERFLOW 0x0200 /**< receive ringbuffer overflow */
#define CAN_NO_DATA 0x0100 /**< no receive data available */

/*************************************************************************
Function: can_peek()
Purpose: Returns the next byte (character) of incoming UART data without
removing it from the ring buffer. That is, successive calls to
uartN_peek() will return the same character, as will the next
call to can_getc()
Returns: lower byte: next byte in ring buffer
higher byte: last receive error
**************************************************************************/
uint16_t can_peek(uint8_t buffer)
{
	uint16_t tmptail;
	uint8_t data;

	if(buffer==0)
	{
		if ( CAN_RxHead == CAN_RxTail ) {
			return CAN_NO_DATA; /* no data available */
		}

		tmptail = (CAN_RxTail + 1) & CAN_BUFFER_MASK;

		/* get data from receive buffer */
		data = CAN_Buf[tmptail];

		return (CAN_LastRxError << 8) + data;
	}
	else
	{
		if ( CAN_RxHead1 == CAN_RxTail1 ) {
			return CAN_NO_DATA; /* no data available */
		}

		tmptail = (CAN_RxTail1 + 1) & CAN_BUFFER_MASK;

		/* get data from receive buffer */
		data = CAN_Buf1[tmptail];

		return (CAN_LastRxError1 << 8) + data;
	}

} /* can_peek */

/*************************************************************************
Function: can_getc()
Purpose: return byte from ringbuffer
Returns: lower byte: received byte from ringbuffer
higher byte: last receive error
**************************************************************************/
uint16_t can_getc(uint8_t buffer)
{
	uint16_t tmptail;
	uint8_t data;

	if(buffer==0)
	{
		if ( CAN_RxHead == CAN_RxTail )
		{
			return CAN_NO_DATA; /* no data available */
		}

		/* calculate /store buffer index */
		tmptail = (CAN_RxTail + 1) & CAN_BUFFER_MASK;
		CAN_RxTail = tmptail;

		/* get data from receive buffer */
		data = CAN_Buf[tmptail];

		return (CAN_LastRxError << 8) + data;
	}
	else
	{
		if ( CAN_RxHead1 == CAN_RxTail1 )
		{
			return CAN_NO_DATA; /* no data available */
		}

		/* calculate /store buffer index */
		tmptail = (CAN_RxTail1 + 1) & CAN_BUFFER_MASK;
		CAN_RxTail1 = tmptail;

		/* get data from receive buffer */
		data = CAN_Buf1[tmptail];

		return (CAN_LastRxError1 << 8) + data;
	}

}

/*************************************************************************
Function: can_available()
Purpose: Determine the number of bytes waiting in the receive buffer
Input: buffer
Returns: Integer number of bytes in the receive buffer
**************************************************************************/
uint16_t can_available(uint8_t buffer)
{
	if(buffer==0)
	{
		return (CAN_BUFFER_SIZE + CAN_RxHead - CAN_RxTail) & CAN_BUFFER_MASK;
	}
	else
	{
		return (CAN_BUFFER_SIZE + CAN_RxHead1 - CAN_RxTail1) & CAN_BUFFER_MASK;
	}
}
/* can_available */


/*************************************************************************
Function: can_flush()
Purpose: Flush bytes waiting the receive buffer. Acutally ignores them.
Input: buffer
Returns: None
**************************************************************************/
void can_flush(uint8_t buffer)
{
	if(buffer==0)
	{
		CAN_RxHead = CAN_RxTail;
	}
	else
	{
		CAN_RxHead1 = CAN_RxTail1;
	}
}


/*
 * Receiver thread.
 */
static WORKING_AREA(can_rx_wa, 2048);
static msg_t can_rx(void *p) {
	static BaseSequentialStream *chp;
  EventListener el;
  CANRxFrame rxmsg;
  int i;
  uint16_t tmphead;
  chp = p;

  //(void)p;
  chRegSetThreadName("receiver");
  chEvtRegister(&CAND1.rxfull_event, &el, 0);
  while(!chThdShouldTerminate()) {
    if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(100)) == 0)
      continue;
    while (canReceive(&CAND1, CAN_ANY_MAILBOX, &rxmsg, TIME_IMMEDIATE) == RDY_OK) {
      /* Process message.*/
        unsigned char lastRxError=0;

		if((rxmsg.EID == 0x108)) // matching myID
		{
			for(i=0;i<rxmsg.DLC;i++)
			{
				/* calculate buffer index */
				tmphead = ( CAN_RxHead1 + 1) & CAN_BUFFER_MASK;

				if ( tmphead == CAN_RxTail1 ) {
					/* error: receive buffer overflow */
				    palTogglePad(GPIOB, GPIOB_LED0);
					lastRxError = CAN_BUFFER_OVERFLOW >> 8;
				} else {
					/* store new index */
					CAN_RxHead1 = tmphead;
					/* store received data in buffer */
					CAN_Buf1[tmphead] = rxmsg.data8[i];
				}
				//chprintf(chp,"%c", rxmsg.data8[i]);
			}
			CAN_LastRxError1=lastRxError;
		}
		if((rxmsg.EID == 0x090)) // matching myID
		{
			for(i=0;i<rxmsg.DLC;i++)
			{
				/* calculate buffer index */
				tmphead = ( CAN_RxHead + 1) & CAN_BUFFER_MASK;

				if ( tmphead == CAN_RxTail ) {
					/* error: receive buffer overflow */
				    palTogglePad(GPIOB, GPIOB_LED0);
					lastRxError = CAN_BUFFER_OVERFLOW >> 8;
				} else {
					/* store new index */
					CAN_RxHead = tmphead;
					/* store received data in buffer */
					CAN_Buf[tmphead] = rxmsg.data8[i];
				}
				//chprintf(chp,"%c", rxmsg.data8[i]);
			}
			CAN_LastRxError=lastRxError;
		}
    }
  }
  chEvtUnregister(&CAND1.rxfull_event, &el);
  return 0;
}

/*
 * Transmitter thread.
 */
static WORKING_AREA(can_tx_wa, 256);
static msg_t can_tx(void * p) {
  CANTxFrame txmsg;
  (void)p;
  chRegSetThreadName("transmitter");

  while (!chThdShouldTerminate()) {
	  txmsg.IDE = CAN_IDE_STD;
	  txmsg.EID = 0x108;
	  txmsg.RTR = CAN_RTR_DATA;
	  txmsg.DLC = 8;
	  txmsg.data8[0] = 0x55;
	  txmsg.data8[1] = 0xAA;
	  txmsg.data8[2] = 0x55;
	  txmsg.data8[3] = 0xAA;
	  txmsg.data8[4] = 0x07;
	  txmsg.data8[5] = 0x10;
	  txmsg.data8[6] = 0x00;
	  txmsg.data8[7] = 0x00;
	  canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(100));
	  txmsg.IDE = CAN_IDE_STD;
	  txmsg.EID = 0x108;
	  txmsg.RTR = CAN_RTR_DATA;
	  txmsg.DLC = 4;
	  txmsg.data8[0] = 0x66;
	  txmsg.data8[1] = 0xCC;
	  txmsg.data8[2] = 0x66;
	  txmsg.data8[3] = 0xCC;
	  canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(100));
	  chThdSleepMilliseconds(2000);
  }
  return 0;
}


/*
 * This is a periodic thread that does absolutely nothing except flashing LEDs.
 */
static WORKING_AREA(waThread108, 1024);
static msg_t Thread108(void *arg) {

	(void)arg;
  char convert[8];
  uint8_t lastRx1 = 0;
  double* value;
  float* fvalue;
  int16_t* ivalue;
  chRegSetThreadName("Message108");
  while (TRUE) {
	uint8_t data = 0;
	uint16_t dataStatus = 0;
	while(can_available(1))
	{
		dataStatus = can_getc(1);
		if(!(dataStatus & 0xff00))
		{
			data =  (uint8_t)(dataStatus&0xff);
			if ((lastRx1 == 0xCC) && (data == 0x55))
			{
				if(!(can_peek(1) & 0xff00))
				{
					if(can_available(1) >= 8)
					{
						convert[0] = CAN_Buf1[(CAN_RxTail1+0)&CAN_BUFFER_MASK];
						convert[1] = CAN_Buf1[(CAN_RxTail1+1)&CAN_BUFFER_MASK];
						convert[2] = CAN_Buf1[(CAN_RxTail1+2)&CAN_BUFFER_MASK];
						convert[3] = CAN_Buf1[(CAN_RxTail1+3)&CAN_BUFFER_MASK];
						convert[4] = CAN_Buf1[(CAN_RxTail1+4)&CAN_BUFFER_MASK];
						convert[5] = CAN_Buf1[(CAN_RxTail1+5)&CAN_BUFFER_MASK];
						convert[6] = CAN_Buf1[(CAN_RxTail1+6)&CAN_BUFFER_MASK];
						convert[7] = CAN_Buf1[(CAN_RxTail1+7)&CAN_BUFFER_MASK];

						uint64_t* uvalue = (uint64_t*)convert;
						//if(uvalue[0] == 0x55AA55AA0910B800)
						if(uvalue[0] == 0x00B81009AA55AA55)
						{
							newdata108=1;
						}
							//else if(uvalue[0] == 0x55AA55AA00103400)
						else if(uvalue[0] == 0x00341000AA55AA55)
						{
							newdata108=2;
						}

						/*if(CAN_Buf1[(CAN_RxTail1+1)&CAN_BUFFER_MASK] == 0xAA)
						{
							if(CAN_Buf1[(CAN_RxTail1+2)&CAN_BUFFER_MASK] == 0x55)
							{
								if(CAN_Buf1[(CAN_RxTail1+3)&CAN_BUFFER_MASK] == 0xAA)
								{
									if(CAN_Buf1[(CAN_RxTail1+4)&CAN_BUFFER_MASK] == 0x09)
									{
										if(CAN_Buf1[(CAN_RxTail1+5)&CAN_BUFFER_MASK] == 0x10)
										{
											if(CAN_Buf1[(CAN_RxTail1+6)&CAN_BUFFER_MASK] == 0xB8)
											{
												if(CAN_Buf1[(CAN_RxTail1+7)&CAN_BUFFER_MASK] == 0x00)
												{
													newdata108=1;
												}
											}
										}
									}
									else if(CAN_Buf1[(CAN_RxTail1+4)&CAN_BUFFER_MASK] == 0x00)
									{
										if(CAN_Buf1[(CAN_RxTail1+5)&CAN_BUFFER_MASK] == 0x10)
										{
											if(CAN_Buf1[(CAN_RxTail1+6)&CAN_BUFFER_MASK] == 0x34)
											{
												if(CAN_Buf1[(CAN_RxTail1+7)&CAN_BUFFER_MASK] == 0x00)
												{
													newdata108=2;
												}
											}
										}
									}
								}
							}
						}*/
						if(newdata108==1)
						{
							while(can_available(1)<(0xB8+8)){
								chThdSleepMicroseconds(1);
							}
                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 24 + 0 + 8) & CAN_BUFFER_MASK];
                            convert[1] = CAN_Buf1[(CAN_RxTail1 + 24 + 1 + 8) & CAN_BUFFER_MASK];
							ivalue= (int16_t*)convert;

							//A
                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 26 + 0 + 8) & CAN_BUFFER_MASK];
                            convert[1] = CAN_Buf1[(CAN_RxTail1 + 26 + 1 + 8) & CAN_BUFFER_MASK];
							ivalue= (int16_t*)convert;
							if(ivalue[0]>-1100 && ivalue[0]<1100)
							{
								telem.roll_raw=ivalue[0];
							}
							telem.input[0]=telem.roll_raw/2+1500;
							//E
                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 28 + 0 + 8) & CAN_BUFFER_MASK];
                            convert[1] = CAN_Buf1[(CAN_RxTail1 + 28 + 1 + 8) & CAN_BUFFER_MASK];
							ivalue= (int16_t*)convert;
							if(ivalue[0]>-1100 && ivalue[0]<1100)
							{
								telem.pitch_raw=ivalue[0];
							}
							telem.input[1]=telem.pitch_raw/2+1500;

							//R
                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 30 + 0 + 8) & CAN_BUFFER_MASK];
                            convert[1] = CAN_Buf1[(CAN_RxTail1 + 30 + 1 + 8) & CAN_BUFFER_MASK];
							ivalue= (int16_t*)convert;
							if(ivalue[0]>-1100 && ivalue[0]<1100)
							{
								telem.yaw_raw=ivalue[0];
							}
							telem.input[2]=telem.yaw_raw/2+1500;

							//U
                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 32 + 0 + 8) & CAN_BUFFER_MASK];
                            convert[1] = CAN_Buf1[(CAN_RxTail1 + 32 + 1 + 8) & CAN_BUFFER_MASK];
							ivalue= (int16_t*)convert;

							//T
                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 34 + 0 + 8) & CAN_BUFFER_MASK];
                            convert[1] = CAN_Buf1[(CAN_RxTail1 + 34 + 1 + 8) & CAN_BUFFER_MASK];
							ivalue= (int16_t*)convert;
							if(ivalue[0]>-1100 && ivalue[0]<1100)
							{
								telem.th_raw=ivalue[0];
							}
							telem.input[3]=telem.th_raw/2+1500;
							telem.input[4]=1500;
							telem.input[5]=1500;
							telem.input[6]=1500;
							telem.input[7]=1500;

                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 36 + 0 + 8) & CAN_BUFFER_MASK];
                            convert[1] = CAN_Buf1[(CAN_RxTail1 + 36 + 1 + 8) & CAN_BUFFER_MASK];
							ivalue= (int16_t*)convert;

                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 38 + 0 + 8) & CAN_BUFFER_MASK];
                            convert[1] = CAN_Buf1[(CAN_RxTail1 + 38 + 1 + 8) & CAN_BUFFER_MASK];
							ivalue= (int16_t*)convert;

                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 40 + 0 + 8) & CAN_BUFFER_MASK];
                            convert[1] = CAN_Buf1[(CAN_RxTail1 + 40 + 1 + 8) & CAN_BUFFER_MASK];
							ivalue= (int16_t*)convert;

                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 42 + 0 + 8) & CAN_BUFFER_MASK];
                            convert[1] = CAN_Buf1[(CAN_RxTail1 + 42 + 1 + 8) & CAN_BUFFER_MASK];
							ivalue= (int16_t*)convert;

							while(can_available(1)<(0xB8+8));
							convert[0]=CAN_Buf1[(CAN_RxTail1+55+0+8)&CAN_BUFFER_MASK];
							telem.fmode = convert[0];

							convert[0]=CAN_Buf1[(CAN_RxTail1+64+0+8)&CAN_BUFFER_MASK];
							convert[1]=CAN_Buf1[(CAN_RxTail1+64+1+8)&CAN_BUFFER_MASK];
							convert[2]=CAN_Buf1[(CAN_RxTail1+64+2+8)&CAN_BUFFER_MASK];
							convert[3]=CAN_Buf1[(CAN_RxTail1+64+3+8)&CAN_BUFFER_MASK];
							convert[4]=CAN_Buf1[(CAN_RxTail1+64+4+8)&CAN_BUFFER_MASK];
							convert[5]=CAN_Buf1[(CAN_RxTail1+64+5+8)&CAN_BUFFER_MASK];
							convert[6]=CAN_Buf1[(CAN_RxTail1+64+6+8)&CAN_BUFFER_MASK];
							convert[7]=CAN_Buf1[(CAN_RxTail1+64+7+8)&CAN_BUFFER_MASK];
							value= (double*)convert;
							telem.homelat = value[0];

							convert[0]=CAN_Buf1[(CAN_RxTail1+72+0+8)&CAN_BUFFER_MASK];
							convert[1]=CAN_Buf1[(CAN_RxTail1+72+1+8)&CAN_BUFFER_MASK];
							convert[2]=CAN_Buf1[(CAN_RxTail1+72+2+8)&CAN_BUFFER_MASK];
							convert[3]=CAN_Buf1[(CAN_RxTail1+72+3+8)&CAN_BUFFER_MASK];
							convert[4]=CAN_Buf1[(CAN_RxTail1+72+4+8)&CAN_BUFFER_MASK];
							convert[5]=CAN_Buf1[(CAN_RxTail1+72+5+8)&CAN_BUFFER_MASK];
							convert[6]=CAN_Buf1[(CAN_RxTail1+72+6+8)&CAN_BUFFER_MASK];
							convert[7]=CAN_Buf1[(CAN_RxTail1+72+7+8)&CAN_BUFFER_MASK];
							value= (double*)convert;
							telem.homelon = value[0];

                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 100 + 0 + 8) & CAN_BUFFER_MASK];
                            telem.status1 = convert[0];
                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 101 + 0 + 8) & CAN_BUFFER_MASK];
                            telem.status2 = convert[0];
                            convert[0] = CAN_Buf1[(CAN_RxTail1 + 102 + 0 + 8) & CAN_BUFFER_MASK];
                            telem.status3 = convert[0];

							convert[0]=CAN_Buf1[(CAN_RxTail1+116+0+8)&CAN_BUFFER_MASK];
							convert[1]=CAN_Buf1[(CAN_RxTail1+116+1+8)&CAN_BUFFER_MASK];
							ivalue = (int16_t*)convert;
							telem.battery=(uint16_t)(ivalue[0]);

							convert[0]=CAN_Buf1[(CAN_RxTail1+172+0+8)&CAN_BUFFER_MASK];
							convert[1]=CAN_Buf1[(CAN_RxTail1+172+1+8)&CAN_BUFFER_MASK];
							convert[2]=CAN_Buf1[(CAN_RxTail1+172+2+8)&CAN_BUFFER_MASK];
							convert[3]=CAN_Buf1[(CAN_RxTail1+172+3+8)&CAN_BUFFER_MASK];
							fvalue= (float*)convert;
							telem.baroaltitude = fvalue[0];

							newdata108=0;
							CAN_RxTail1 = (CAN_RxTail1+0xB8+8+2)&CAN_BUFFER_MASK;
						} else if(newdata108==2) {
							while(can_available(1)<(0x34+8)){
								chThdSleepMicroseconds(1);
							}
							newdata108=0;
							CAN_RxTail1 = (CAN_RxTail1+0x34+8+2)&CAN_BUFFER_MASK;
						}
					}
				}
			}
			lastRx1=data;
		}
	}
	chThdSleepMilliseconds(10);
  }
  return 0;
}


/*
 * This is a periodic thread that does absolutely nothing except flashing LEDs.
 */
static WORKING_AREA(waThread090, 1024);
static msg_t Thread090(void *arg) {

	(void)arg;
  char convert[8];
  uint8_t lastRx = 0;
  double* value;
  float* fvalue;
  int16_t* ivalue;
  chRegSetThreadName("Message090");
  while (TRUE) {
	uint8_t data = 0;
	uint16_t dataStatus = 0;
	while(can_available(0))
	{
		dataStatus = can_getc(0);
		if(!(dataStatus & 0xff00))
		{
			data =  (uint8_t)(dataStatus&0xff);
			if ((lastRx == 0xCC) && (data == 0x55))
			{
				if(!(can_peek(0) & 0xff00))
				{
					if(can_available(0) >= 8)
					{
						convert[0] = CAN_Buf[(CAN_RxTail+0)&CAN_BUFFER_MASK];
						convert[1] = CAN_Buf[(CAN_RxTail+1)&CAN_BUFFER_MASK];
						convert[2] = CAN_Buf[(CAN_RxTail+2)&CAN_BUFFER_MASK];
						convert[3] = CAN_Buf[(CAN_RxTail+3)&CAN_BUFFER_MASK];
						convert[4] = CAN_Buf[(CAN_RxTail+4)&CAN_BUFFER_MASK];
						convert[5] = CAN_Buf[(CAN_RxTail+5)&CAN_BUFFER_MASK];
						convert[6] = CAN_Buf[(CAN_RxTail+6)&CAN_BUFFER_MASK];
						convert[7] = CAN_Buf[(CAN_RxTail+7)&CAN_BUFFER_MASK];

						uint64_t* uvalue = (uint64_t*)convert;
						//if(uvalue[0] == 0x55AA55AA02107800)
						if(uvalue[0] == 0x00781002AA55AA55)
						{
							newdata090=1;
						}
						//else if(uvalue[0] == 0x55AA55AA01100E00)
						else if(uvalue[0] == 0x000E1001AA55AA55)
						{
							newdata090=2;
						}

						/*if(CAN_Buf[(CAN_RxTail+1)&CAN_BUFFER_MASK] == 0xAA)
						{
							if(CAN_Buf[(CAN_RxTail+2)&CAN_BUFFER_MASK] == 0x55)
							{
								if(CAN_Buf[(CAN_RxTail+3)&CAN_BUFFER_MASK] == 0xAA)
								{
									if(CAN_Buf[(CAN_RxTail+4)&CAN_BUFFER_MASK] == 0x02)
									{
										if(CAN_Buf[(CAN_RxTail+5)&CAN_BUFFER_MASK] == 0x10)
										{
											if(CAN_Buf[(CAN_RxTail+6)&CAN_BUFFER_MASK] == 0x78)
											{
												if(CAN_Buf[(CAN_RxTail+7)&CAN_BUFFER_MASK] == 0x00)
												{
													newdata090=1;
												}
											}
										}
									}
									else if(CAN_Buf[(CAN_RxTail+4)&CAN_BUFFER_MASK] == 0x01)
									{
										if(CAN_Buf[(CAN_RxTail+5)&CAN_BUFFER_MASK] == 0x10)
										{
											if(CAN_Buf[(CAN_RxTail+6)&CAN_BUFFER_MASK] == 0x0E)
											{
												if(CAN_Buf[(CAN_RxTail+7)&CAN_BUFFER_MASK] == 0x00)
												{
													newdata090=2;
												}
											}
										}
									}
								}
							}
						}*/
						if(newdata090==1)
						{
							while(can_available(0)<(0x78+8)){
								chThdSleepMicroseconds(1);
							}
							convert[0]=CAN_Buf[(CAN_RxTail+0+0+8)&CAN_BUFFER_MASK];
							convert[1]=CAN_Buf[(CAN_RxTail+0+1+8)&CAN_BUFFER_MASK];
							convert[2]=CAN_Buf[(CAN_RxTail+0+2+8)&CAN_BUFFER_MASK];
							convert[3]=CAN_Buf[(CAN_RxTail+0+3+8)&CAN_BUFFER_MASK];
							convert[4]=CAN_Buf[(CAN_RxTail+0+4+8)&CAN_BUFFER_MASK];
							convert[5]=CAN_Buf[(CAN_RxTail+0+5+8)&CAN_BUFFER_MASK];
							convert[6]=CAN_Buf[(CAN_RxTail+0+6+8)&CAN_BUFFER_MASK];
							convert[7]=CAN_Buf[(CAN_RxTail+0+7+8)&CAN_BUFFER_MASK];
							value= (double*)convert;
							telem.lon = value[0];

							convert[0]=CAN_Buf[(CAN_RxTail+8+0+8)&CAN_BUFFER_MASK];
							convert[1]=CAN_Buf[(CAN_RxTail+8+1+8)&CAN_BUFFER_MASK];
							convert[2]=CAN_Buf[(CAN_RxTail+8+2+8)&CAN_BUFFER_MASK];
							convert[3]=CAN_Buf[(CAN_RxTail+8+3+8)&CAN_BUFFER_MASK];
							convert[4]=CAN_Buf[(CAN_RxTail+8+4+8)&CAN_BUFFER_MASK];
							convert[5]=CAN_Buf[(CAN_RxTail+8+5+8)&CAN_BUFFER_MASK];
							convert[6]=CAN_Buf[(CAN_RxTail+8+6+8)&CAN_BUFFER_MASK];
							convert[7]=CAN_Buf[(CAN_RxTail+8+7+8)&CAN_BUFFER_MASK];
							value= (double*)convert;
							telem.lat = value[0];

							convert[0] = CAN_Buf[(CAN_RxTail+16+0+8)&CAN_BUFFER_MASK];
							convert[1] = CAN_Buf[(CAN_RxTail+16+1+8)&CAN_BUFFER_MASK];
							convert[2] = CAN_Buf[(CAN_RxTail+16+2+8)&CAN_BUFFER_MASK];
							convert[3] = CAN_Buf[(CAN_RxTail+16+3+8)&CAN_BUFFER_MASK];
							fvalue = (float*)convert;
							telem.galt=fvalue[0];

							quaternion_t quat;
							euler_t euler;

							convert[0] = CAN_Buf[(CAN_RxTail+48+0+8)&CAN_BUFFER_MASK];
							convert[1] = CAN_Buf[(CAN_RxTail+48+1+8)&CAN_BUFFER_MASK];
							convert[2] = CAN_Buf[(CAN_RxTail+48+2+8)&CAN_BUFFER_MASK];
							convert[3] = CAN_Buf[(CAN_RxTail+48+3+8)&CAN_BUFFER_MASK];
							fvalue = (float*)convert;
							quat.q0=fvalue[0];
							q[0]=fvalue[0];

							convert[0] = CAN_Buf[(CAN_RxTail+52+0+8)&CAN_BUFFER_MASK];
							convert[1] = CAN_Buf[(CAN_RxTail+52+1+8)&CAN_BUFFER_MASK];
							convert[2] = CAN_Buf[(CAN_RxTail+52+2+8)&CAN_BUFFER_MASK];
							convert[3] = CAN_Buf[(CAN_RxTail+52+3+8)&CAN_BUFFER_MASK];
							fvalue = (float*)convert;
							quat.q1=fvalue[0];
							q[1]=fvalue[0];

							convert[0] = CAN_Buf[(CAN_RxTail+56+0+8)&CAN_BUFFER_MASK];
							convert[1] = CAN_Buf[(CAN_RxTail+56+1+8)&CAN_BUFFER_MASK];
							convert[2] = CAN_Buf[(CAN_RxTail+56+2+8)&CAN_BUFFER_MASK];
							convert[3] = CAN_Buf[(CAN_RxTail+56+3+8)&CAN_BUFFER_MASK];
							fvalue = (float*)convert;
							quat.q2=fvalue[0];
							q[2]=fvalue[0];

							convert[0] = CAN_Buf[(CAN_RxTail+60+0+8)&CAN_BUFFER_MASK];
							convert[1] = CAN_Buf[(CAN_RxTail+60+1+8)&CAN_BUFFER_MASK];
							convert[2] = CAN_Buf[(CAN_RxTail+60+2+8)&CAN_BUFFER_MASK];
							convert[3] = CAN_Buf[(CAN_RxTail+60+3+8)&CAN_BUFFER_MASK];
							fvalue = (float*)convert;
							quat.q3=fvalue[0];
							q[3]=fvalue[0];

							quaternion_to_euler(&quat,&euler);
							telem.Roll = euler.roll;
							telem.Pitch = euler.pitch;
							telem.Yaw = euler.yaw;

							convert[0] = CAN_Buf[(CAN_RxTail+76+0+8)&CAN_BUFFER_MASK];
							convert[1] = CAN_Buf[(CAN_RxTail+76+1+8)&CAN_BUFFER_MASK];
							convert[2] = CAN_Buf[(CAN_RxTail+76+2+8)&CAN_BUFFER_MASK];
							convert[3] = CAN_Buf[(CAN_RxTail+76+3+8)&CAN_BUFFER_MASK];
							fvalue = (float*)convert;
							telem.vN = fvalue[0];

							convert[0] = CAN_Buf[(CAN_RxTail+80+0+8)&CAN_BUFFER_MASK];
							convert[1] = CAN_Buf[(CAN_RxTail+80+1+8)&CAN_BUFFER_MASK];
							convert[2] = CAN_Buf[(CAN_RxTail+80+2+8)&CAN_BUFFER_MASK];
							convert[3] = CAN_Buf[(CAN_RxTail+80+3+8)&CAN_BUFFER_MASK];
							fvalue = (float*)convert;
							telem.vE = fvalue[0];

							telem.gpsspeed=sqrtf(powf(telem.vN,2)+powf(telem.vE,2));

							convert[0] = CAN_Buf[(CAN_RxTail+84+0+8)&CAN_BUFFER_MASK];
							convert[1] = CAN_Buf[(CAN_RxTail+84+1+8)&CAN_BUFFER_MASK];
							convert[2] = CAN_Buf[(CAN_RxTail+84+2+8)&CAN_BUFFER_MASK];
							convert[3] = CAN_Buf[(CAN_RxTail+84+3+8)&CAN_BUFFER_MASK];
							fvalue = (float*)convert;
							telem.vD = fvalue[0];

							convert[0]=CAN_Buf[(CAN_RxTail+116+0+8)&CAN_BUFFER_MASK];
							convert[1]=CAN_Buf[(CAN_RxTail+116+1+8)&CAN_BUFFER_MASK];
							ivalue = (int16_t*)convert;
							telem.sats=(uint16_t)(ivalue[0]);


							newdata090=0;
							CAN_RxTail = (CAN_RxTail+0x78+8+2)&CAN_BUFFER_MASK;
							haversine_m();
						}
						else if(newdata090==2)
						{
							while(can_available(0)<(0x0E + 8)){
								chThdSleepMicroseconds(1);
							}

							newdata090=0;
							CAN_RxTail = (CAN_RxTail+0x0E + 8+2)&CAN_BUFFER_MASK;
						}
					}
				}
			}
			lastRx=data;
		}
	}
	chThdSleepMilliseconds(10);
  }
  return 0;
}

/*
 * 1000KBaud, automatic wakeup, automatic recover
 * from abort mode.
 */
static const CANConfig cancfg = {
  CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
  CAN_BTR_SJW(0) | CAN_BTR_TS2(4) |
  CAN_BTR_TS1(11) | CAN_BTR_BRP(1)
};

void initNazaCAN(void)
{
	/*
	* Activates the CAN driver 1.
	*/
   canStart(&CAND1, &cancfg);

   /*
	* Starting the transmitter and receiver threads.
	*/
   //chThdCreateStatic(can_rx_wa, sizeof(can_rx_wa), LOWPRIO + 7, can_rx, (void *)&SDU1);
   chThdCreateStatic(can_rx_wa, sizeof(can_rx_wa), LOWPRIO + 7, can_rx, NULL);
   chThdCreateStatic(can_tx_wa, sizeof(can_tx_wa), LOWPRIO + 7, can_tx, NULL);

   /*
	* Creates the example thread.
	*/
   chThdCreateStatic(waThread108, sizeof(waThread108), LOWPRIO, Thread108, NULL);
   chThdCreateStatic(waThread090, sizeof(waThread090), LOWPRIO, Thread090, NULL);
}




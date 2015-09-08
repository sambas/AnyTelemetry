/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "common.h"
#include "ch.h"
#include "hal.h"
#include "test.h"
#include "shell.h"
#include "chprintf.h"

#include "mwserial.h"
#include "mwserialclient.h"
#include "nazacan.h"
#include "usbcomm.h"
#include "frskytelemetry.h"
#include "frskyhubtelemetry.h"


TELEMETRY telem;
float q[4];
core_t core;
extern SerialUSBDriver SDU1;
/*
 * Debug print thread.
 */
static WORKING_AREA(waDbg_print, 1024);
static msg_t Dbg_print(void *p) {
  static BaseSequentialStream *chp;
  chp = p;
  chRegSetThreadName("printer");
  chThdSleepMilliseconds(2000);
  /*chprintf(chp,"hlat");
  chprintf(chp,"\thlon");
  chprintf(chp,"\tlat");
  chprintf(chp,"\tlon");
  chprintf(chp,"\tsats");
  chprintf(chp,"\tvoltage\r\n");
  while (!chThdShouldTerminate()) {
	  chprintf(chp,"%9D",(long)(telem.homelat*180.0f/M_PI*10e6f));
	  chprintf(chp,"\t%9D",(long)(telem.homelon*180.0f/M_PI*10e6f));
	  chprintf(chp,"\t%9D",(long)(telem.lat*180.0f/M_PI*10e6f));
	  chprintf(chp,"\t%9D",(long)(telem.lon*180.0f/M_PI*10e6f));
	  chprintf(chp,"\t%d",telem.sats);
	  chprintf(chp,"\t%d\r\n",telem.battery);
	  chThdSleepMilliseconds(500);
  }*/


  /*chprintf(chp,"q0");
  chprintf(chp,"\tq1");
  chprintf(chp,"\tq2");
  chprintf(chp,"\tq3");*/
  while (!chThdShouldTerminate()) {
	  /*chprintf(chp,"%4D",(long)(q[0]*1000));
	  chprintf(chp,"\t%4D",(long)(q[1]*1000));
	  chprintf(chp,"\t%4D",(long)(q[2]*1000));
	  chprintf(chp,"\t%4D\r\n",(long)(q[3]*1000));*/
	  /*chThdSleepMilliseconds(500);
	  telem.gpsspeed=1;
	  telem.baroaltitude=10;
	  telem.galt=180;
	  telem.battery=9000;
	  telem.sats=4;
	  telem.lat=1.0951417;
	  telem.lon=0.4825137;
	  telem.th_raw=0x7D00;
	  chThdSleepMilliseconds(500);
	  telem.gpsspeed=2;
	  telem.baroaltitude=20;
	  telem.galt=160;
	  telem.battery=9000;
	  telem.sats=5;
	  telem.lat=1.0951417;
	  telem.lon=0.4825137;
	  telem.th_raw=100;
	  telem.Yaw=120.0;
	  chThdSleepMilliseconds(500);
	  telem.gpsspeed=10;
	  telem.baroaltitude=30;
	  telem.galt=140;
	  telem.battery=7000;
	  telem.sats=6;
	  telem.lon=1.0951417;
	  telem.lat=0.4825137;
	  telem.th_raw=0x007E;
	  chThdSleepMilliseconds(500);
	  telem.gpsspeed=20;
	  telem.baroaltitude=40;
	  telem.galt=120;
	  telem.battery=7000;
	  telem.sats=7;
	  telem.lon=1.0951417;
	  telem.lat=0.4825137;
	  telem.th_raw=0x7D7E;
	  telem.Yaw=20.0;*/

	  chprintf(chp,"1:%4D.%4D.%4D\r\n",(int16_t)(telem.Roll),(int16_t)(telem.Pitch),(int16_t)(telem.Yaw));
	  chprintf(chp,"2:%4D.%4D.%4D\r\n",telem.MwVersion,(int16_t)(telem.baroaltitude),(int16_t)(telem.sats));
	  chThdSleepMilliseconds(100);
  }
  return 0;
}


static WORKING_AREA(waSerial2_Thread, 1024);
static msg_t Serial2_Thread(void *p) {
  EventListener s2EventListener;

  (void)p;
  chEvtRegisterMask(chnGetEventSource(&SD2), &s2EventListener, CHN_INPUT_AVAILABLE);
  while (TRUE) {
	flagsmask_t flags;

    chEvtWaitOne(CHN_INPUT_AVAILABLE);
    flags = chEvtGetAndClearFlags(&s2EventListener);
    if (flags & CHN_INPUT_AVAILABLE) {
      /* Data available read here.*/
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



static const SerialConfig sd2_config =
{
  115200,
  0,
  USART_CR2_STOP1_BITS | USART_CR2_LINEN,
  0
};

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

#ifndef NAZA
  int i;
  for (i = 0; i < 8; i++)
      telem.input[i]=1500;
#endif

#ifdef NAZA
  initNazaCAN();
  initMwSerial(); //OSD
  initFrskyTelemetry(); //X?R
#elif defined(RIDGE)
  initMwSerial(); //OSD
  initMwSerialClient(); //ANYFC
  initFrskyTelemetry(); //X?R
#elif defined(D8R)
  initMwSerialClient(); //ANYFC
  initFrskyHubTelemetry(); //D?R
#endif
  initUsbComm();

  /*
   * Activates the serial driver 2 using the default configuration, pins
   * are pre-configured in board.h.
   */
  //sdStart(&SD2, &sd2_config);

  chThdCreateStatic(waDbg_print, sizeof(waDbg_print), LOWPRIO, Dbg_print, (void *)&SDU1);
  //chThdCreateStatic(waDbg_print, sizeof(waDbg_print), LOWPRIO, Dbg_print, NULL);
  //chThdCreateStatic(waSerial2_Thread, sizeof(waSerial2_Thread), LOWPRIO, Serial2_Thread, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state, when the button is
   * pressed the test procedure is launched.
   */
  while (TRUE) {

	//if (!shelltp && (SDU1.config->usbp->state == USB_ACTIVE))
	//  shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
	//else if (chThdTerminated(shelltp)) {
	//  chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
	//  shelltp = NULL;           /* Triggers spawning of a new shell.        */
	//}

    //if (palReadPad(GPIOA, GPIOA_WKUP_BUTTON))
    //  TestThread(&SDU1);
    chThdSleepMilliseconds(500);
  }
}

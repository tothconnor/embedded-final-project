/***********************************************************************
MODULE:    UART
VERSION:   1.03
CONTAINS:  Routines for controlling the UART peripheral on the Philips
           P89LPC932
COPYRIGHT: Embedded Systems Academy, Inc. - www.esacademy.com
LICENSE:   May be freely used in commercial and non-commercial code
           without royalties provided this copyright notice remains
           in this file and unaltered
WARNING:   IF THIS FILE IS REGENERATED BY CODE ARCHITECT ANY CHANGES
           MADE WILL BE LOST. WHERE POSSIBLE USE ONLY CODE ARCHITECT
           TO CHANGE THE CONTENTS OF THIS FILE
GENERATED: On "Jun 24 2003" at "10:17:56" by Code Architect 2.01
***********************************************************************/

// SFR description needs to be included
#include <reg932.h>
#include "uart.h"

// flag that indicates if the UART is busy transmitting or not
static bit mtxbusy;

/***********************************************************************
DESC:    Initializes UART for mode 1
         Baudrate: 9600
RETURNS: Nothing
CAUTION: If interrupts are being used then EA must be set to 1
         after calling this function
************************************************************************/
void uart_init
  (
  void
  )
{
  // configure UART
  // clear SMOD0
  PCON &= ~0x40;
  SCON = 0x50;
  // set or clear SMOD1
  PCON &= 0x7f;
  PCON |= (0 << 8);
  SSTAT = 0x00;

  // enable break detect
  AUXR1 |= 0x40;

  // configure baud rate generator
  BRGCON = 0x00;
  BRGR0 = 0xF0;
  BRGR1 = 0x02;                      //BRGR0 and BRGR1 were
  BRGCON = 0x03;                     //changed to set the baud rate to 9600

  // TxD = push-pull, RxD = input
  P1M1 &= ~0x01;
  P1M2 |= 0x01;
  P1M1 |= 0x02;
  P1M2 &= ~0x02;

  // initially not busy
  mtxbusy = 0;

  // set isr priority to 0
  IP0 &= 0xEF;
  IP0H &= 0xEF;
  // enable uart interrupt
  ES = 1;

} // uart_init

/***********************************************************************
DESC:    UART Interrupt Service Routine
RETURNS: Nothing
CAUTION: uart_init must be called first
         EA must be set to 1
************************************************************************/
void uart_isr
  (
  void
  ) interrupt 4 using 1
{
  if (RI)
  {
    // clear interrupt flag
    RI = 0;
  } // if

  if (TI)
  {
    // clear interrupt flag
    TI = 0;
    // no longer busy
    mtxbusy = 0;
  } // if

} // uart_isr

/***********************************************************************
DESC:    Transmits a 8-bit value via the UART in the current mode
         May result in a transmit interrupt if enabled.
RETURNS: Nothing
CAUTION: uart_init must be called first
************************************************************************/
void uart_transmit
  (
  unsigned char value    // data to transmit
  )
{
  while(mtxbusy);
  mtxbusy = 1;
  SBUF = value;
} // uart_transmit

/***********************************************************************
DESC:    Gets a received 8-bit value from the UART
RETURNS: Received data
CAUTION: uart_init must be called first
************************************************************************/

//This was commented out by Connor J. Toth
/*unsigned char uart_get
  (
  void
  )
{
  return SBUF;
} // uart_get*/


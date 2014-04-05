/*--------------------------------------------------------------------------*
* Hardware interface for Microboot
*---------------------------------------------------------------------------*
* 01-Apr-2014 ShaneG
*
*  Initial version.
*--------------------------------------------------------------------------*/
#ifndef __MICROBOOT_H
#define __MICROBOOT_H

/** Initialise the UART
 */
void uartInit();

/** Shutdown the UART
 */
void uartDone();

/** Receive a single character
 *
 * Wait for a single character on the UART and return it.
 *
 * @return the character received.
 */
char uartRecv();

/** Write a single character
 *
 * Send a single character on the UART.
 *
 * @param ch the character to send.
 */
void uartSend(char ch);

#endif /* __MICROBOOT_H */

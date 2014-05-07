
// Based on code published at http://learningmsp430.wordpress.com/2014/01/08/serial-communication/

#include "serial.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    uart_init();
    lcd_init();
    IE2 |= UCA0RXIE;
    println("START");
    __bis_SR_register(LPM0_bits + GIE);
}
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    if(UCA0RXBUF == '0')
        {
            send_command(0x01);
        }
    else
        send_data(UCA0RXBUF);
}


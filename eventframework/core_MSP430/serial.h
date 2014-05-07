/*
 *  serial.h
 *  Created on     : 01-Jan-2014 12:25:06 PM
 *  Author     : Manpreet Singh Minhas
 *  Website        : http://learningmsp430.wordpress.com/
 *  This is a standard header for 9600 baud rate serial communication.
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include<msp430g2553.h>// Change this as per your micro-controller chip.
void uart_init(void);
void send_byte(int data);
void print(char *data);
void println(char *data);
void send_int(int a);
void send_intln(int a);
void uart_init()
{
    P1SEL  |= BIT1|BIT2; // Port for UART transmission and reception purpose.
    P1SEL2 |= BIT1|BIT2;
    UCA0CTL1 |= UCSWRST; // Software reset
    UCA0CTL1 |= UCSSEL_1;// Select ACLK
    UCA0BR0 = 3;// This is the count for getting 9600 baud rate 32768/9600 = 3.4133
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS1 + UCBRS0;// Modulation bits = 0b00000011
    UCA0CTL1 &= ~UCSWRST;   // Start the UART
}
void send_byte(int data)
{
    while (!(IFG2&UCA0TXIFG));
    UCA0TXBUF = data;
}
void print(char *data)
{
    while(*data)
    {
        send_byte(*data);
        data++;
    }
}
void println(char *data)
{
    while(*data)
    {
        send_byte(*data);
        data++;
    }
    send_byte('\n');
    send_byte('\r');
}
void send_int(int a)
{
    int temp;
    int rev=0;
    int dummy =a;
     while (dummy)
       {
          rev = rev * 10;
          rev = rev + dummy%10;
          dummy = dummy/10;
       }
    while(rev)
    {
        temp=rev%10;
        send_byte(0x30+temp);
        rev /=10;
    }
}
void send_intln(int a)
{
    int temp;
    int rev=0;
    int dummy =a;
     while (dummy)
       {
          rev = rev * 10;
          rev = rev + dummy%10;
          dummy = dummy/10;
       }
    while(rev)
    {
        temp=rev%10;
        send_byte(0x30+temp);
        rev /=10;
    }
     send_byte('\n');
     send_byte('\r');
}
#endif/* SERIAL_H_ */

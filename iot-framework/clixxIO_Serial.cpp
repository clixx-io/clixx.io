/*
 * Copyright (c) 2014 clixx.io
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the Clixx.io nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CLIXX.IO BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>

#include "clixxIO.hpp"

clixxIOSerial Serial;

/**
 * Adds a character to the input buffer
 */
void clixxIOSerial::addbufferchar(char c)
{

    C_serialchar(pMainClass);

    if (c == '\r')
    {
        processcommand();
        linebufferpos = 0;

    } else
    {
        if (linebufferpos < BUFFSIZE_LINELEN){

            linebuffer[linebufferpos++] = c;
            linebuffer[linebufferpos] = (char ) 0;

        }
    }

}

/**
 * process the input buffer based on a full line of text
 */
void clixxIOSerial::processcommand(void)
{
    if (iotmode != 0){
        C_iotmessage(pMainClass);
    }
    else {
        C_serialline(pMainClass);
    }

}

/**
 * Start publishing on a topic. This transmits the
 * name of the topic to the host.
 */
int clixxIOSerial::beginPublishing(const char *topic){

    #ifdef TARGET_AVR
    echo = linemode = (unsigned char) 0x01; 
    #endif
    
    iotmode = 1;

    write('@');
    puts(topic);
    puts("\r\n");

    return 0;

}

/**
 * Publish an Integer value to the host. This transmits the
 * text up to the host using an escape.
 */
int clixxIOSerial::publish(const int publishint){

    char buffer[10];
    
    puts("!");
    snprintf(buffer, sizeof(buffer), "%d", publishint);
    puts((const char *) &buffer);
    puts("\r\n");

    return(0);
}

/**
 * Publish a text value to the host. This transmits the
 * text up to the host using an escape.
 */
int clixxIOSerial::publish(const char *publishtext){

    puts("!\"");
    puts(publishtext);
    puts("\"\r\n");

    return(0);
}

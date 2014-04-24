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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <limits.h>
#include <termios.h>
#include <unistd.h>

#include <getopt.h>

/**********************************************************************/

int serial_feed_setup(const char *portname, long baudrate);
int serial_feed_capture(int tty_fd, char *buffer, int buffersize, int dumpchars = 0);
int serial_feed_close(int tty_fd);
int serial_available(int tty_fd);
int term_setup(int fd, long baudrate);

#ifdef UUCP_LOCK_DIR

/* use HDB UUCP locks  .. see
 * <http://www.faqs.org/faqs/uucp-internals> for details
 */

char lockname[_POSIX_PATH_MAX] = "";

int uucp_lockname(const char *dir, const char *file)
{
	char *p, *cp;
	struct stat sb;

	if ( ! dir || *dir == '\0' || stat(dir, &sb) != 0 )
		return -1;

	/* cut-off initial "/dev/" from file-name */
	p = strchr(file + 1, '/');
	p = p ? p + 1 : (char *)file;
	/* replace '/'s with '_'s in what remains (after making a copy) */
	p = cp = strdup(p);
	do { if ( *p == '/' ) *p = '_'; } while(*p++);
	/* build lockname */
	snprintf(lockname, sizeof(lockname), "%s/LCK..%s", dir, cp);
	/* destroy the copy */
	free(cp);

	return 0;
}

int uucp_lock(void)
{
	int r, fd, pid;
	char buf[16];
	mode_t m;

	if ( lockname[0] == '\0' ) return 0;

	fd = open(lockname, O_RDONLY);
	if ( fd >= 0 ) {
		r = read(fd, buf, sizeof(buf)); 
		close(fd);
		/* if r == 4, lock file is binary (old-style) */
		pid = (r == 4) ? *(int *)buf : strtol(buf, NULL, 10);
		if ( pid > 0 
			 && kill((pid_t)pid, 0) < 0 
			 && errno == ESRCH ) {
			/* stale lock file */
			printf("Removing stale lock: %s\n", lockname);
			sleep(1);
			unlink(lockname);
		} else {
			lockname[0] = '\0';
			errno = EEXIST;
			return -1;
		}
	}
	/* lock it */
	m = umask(022);
	fd = open(lockname, O_WRONLY|O_CREAT|O_EXCL, 0666);
	if ( fd < 0 ) { lockname[0] = '\0'; return -1; }
	umask(m);
	snprintf(buf, sizeof(buf), "%04d\n", getpid());
	write(fd, buf, strlen(buf));
	close(fd);

	return 0;
}

int uucp_unlock(void)
{
	if ( lockname[0] ) unlink(lockname);
	return 0;
}

#endif /* of UUCP_LOCK_DIR */

/**********************************************************************/

void child_empty_handler (int signum)
{
}

void fatal (const char *format, ...)
{
	char buf[256];
	va_list args;
	int len;

	va_start(args, format);
	len = vsnprintf(buf, sizeof(buf), format, args);
	buf[sizeof(buf) - 1] = '\0';
	va_end(args);
	
	puts("\r\nFATAL: ");
	puts(buf);

	/* wait a bit for output to drain */
	sleep(1);

#ifdef UUCP_LOCK_DIR
	uucp_unlock();
#endif
	
	exit(EXIT_FAILURE);
}

void establish_child_signal_handlers (void)
{
	struct sigaction empty_action;
	
	/* Set up the structure to specify the "empty" action. */
    empty_action.sa_handler = child_empty_handler;
	sigemptyset (&empty_action.sa_mask);
	empty_action.sa_flags = 0;
	
	sigaction (SIGINT, &empty_action, NULL);
	sigaction (SIGTERM, &empty_action, NULL);
}

int term_setup(int fd, long baudrate)
{
	struct termios tio;
	
	// Extract parameters
	int r = tcgetattr(fd,&tio);
	if ( r < 0 ) {
		fatal("cannot get serial port paramenters ");
		return(r);
	}
	
	r = cfsetospeed(&tio, baudrate);
	if ( r < 0 ) {
		fatal("cannot Set serial port to baudrate %d: %s", baudrate, strerror(errno));
		return(r);
	}
			
	r = cfsetispeed(&tio, baudrate);
	if ( r < 0 ) {
		fatal("cannot Set serial port to baudrate %d: %s", baudrate, strerror(errno));
		return(r);
	}

	// Set No Parity
	tio.c_cflag = (tio.c_cflag & ~(PARENB | PARODD));

	r = tcsetattr(fd,0,&tio);
	if ( r < 0 ) {
		fatal("cannot Set serial port paramters - %s", strerror(errno));
		return(r);
	}

	return(0);
}

struct pollfd fds[1];

int serial_feed_setup(const char *portname, long baudrate)
{
	establish_child_signal_handlers();

#ifdef UUCP_LOCK_DIR
	if ( ! opts.nolock ) uucp_lockname(UUCP_LOCK_DIR, opts.port);
	if ( uucp_lock() < 0 )
		fatal("cannot lock %s: %s", opts.port, strerror(errno));
#endif

	int tty_fd = open(portname, O_RDWR | O_NONBLOCK);
	if (tty_fd < 0)
		fatal("cannot open %s: %s", portname, strerror(errno));

	int r = term_setup(tty_fd, baudrate);
	if ( r < 0 )
		fatal("failed to setup serial port on device %s:", 
			  portname);

	fds[0].fd = tty_fd;
	fds[0].events = POLLIN ;
	
	return(tty_fd);
}

int serial_available(int tty_fd)
{
	ssize_t rc = 0;
	
	int pollrc = poll( fds, 1, 1000);
	if (pollrc < 0)
	{
		perror("poll");
		return(pollrc);
	}
	else if( pollrc > 0)
	{
		if( fds[0].revents & POLLIN )
			return(1)
	}
	return 0;
}

int serial_feed_capture(int tty_fd, char *buffer, int buffersize, int dumpchars)
{

	ssize_t rc = 0;
	
	int pollrc = poll( fds, 1, 1000);
	if (pollrc < 0)
	{
		perror("poll");
		return(pollrc);
	}
	else if( pollrc > 0)
	{
		if( fds[0].revents & POLLIN )
		{
			rc = read(tty_fd, buffer, buffersize-1 );
			if (rc > 0)
			{
				// Null terminate
				*(buffer+rc) = 0;
				
				// You've got rc characters. do something with buff 
				if (dumpchars)
				{
					int xc;
					for (xc=0;xc<rc;xc++)
						putchar(buffer[xc]); 
				}
			}
		}
	}
	
	/* returns the number of characters read from the port */
	return(rc);
	
}

int serial_feed_close(int tty_fd)
{
	return close(tty_fd);
}


/**
 * 	\file
 *	
 *	\brief XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
 *
 *	\page License
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 *
 *  Created on: 24.08.2011
 *      Author: Anton Glukhov
 */

#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#include "stm32l1xx.h"

#include <errno.h>
#undef errno
extern int errno;

#include "syscalls.h"
#include "stm32l1xx_conf.h"


int _write(int file, char *ptr, int len)
{
	int n;
    switch (file)
    {
    case STDOUT_FILENO: /*stdout*/
        for (n = 0; n < len; n++)
        {
    	    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); /* Waitwhile TX full */
    	    USART_SendData(USART1, (uint8_t) * ptr++);
        }
        break;
    case STDERR_FILENO: /* stderr */
        for (n = 0; n < len; n++)
        {
    	    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); /* Waitwhile TX full */
    	    USART_SendData(USART3, (uint8_t) * ptr++);
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}


int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}


int _getpid()
{
  return 1;
}


int _read(int file, char *ptr, int len)
{
	return 0;
}


void _exit(int status)
{
  _write(STDERR_FILENO, "exit", 4);
  while (1)
  {
    ;
  }
}


int _close(int file)
{
	return -1;
}


int _kill(int pid, int sig)
{
  errno = EINVAL;
  return (-1);
}


/*
 isatty
 Query whether output stream is a terminal. For consistency with the other minimal implementations,
 */
int _isatty(int file) {
    switch (file){
    case STDOUT_FILENO:
    case STDERR_FILENO:
    case STDIN_FILENO:
        return 1;
    default:
        //errno = ENOTTY;
        errno = EBADF;
        return 0;
    }
}


caddr_t _sbrk(int incr)
{
	extern char _end;		/* Defined by linker */
	static char *heap_end;	/* Point to and of heap */
	char *prev_heap_end;

	register char *stack_ptr __asm__ ("sp");

	if (heap_end == 0) {
		heap_end = &_end;
	}

	prev_heap_end = heap_end;
	if (heap_end + incr > stack_ptr) {
	_write (1, "Heap and stack collision\n", 25);
//	abort();
	}
	heap_end += incr;
	return (caddr_t) prev_heap_end;
}


int _lseek(int file, int ptr, int dir)
{
    return 0;
}

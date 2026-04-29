#pragma once

#ifdef _WIN32

#include <fcntl.h>
#include <conio.h>
#include <windows.h>

void setConsoleUTF8();

#    ifdef CROSS_GETCH_IMPLEMENTATION

void setConsoleUTF8()
{
    SetConsoleOutputCP(CP_UTF8);
}

#    endif //CROSS_GETCH_IMPLEMENTATION
#endif //_WIN32

#ifdef __linux__

#include <termios.h>
#include <stdio.h>

char _getch( void );
char getche( void );
void setConsoleUTF8();

#ifdef CROSS_GETCH_IMPLEMENTATION

void setConsoleUTF8() {}

static struct termios old, current;

/* Read 1 character - echo defines echo mode */
static inline char getch_( int echo )
{
    char ch;

    tcgetattr( 0, &old );       /* grab old terminal i/o settings */
    current = old;              /* make new settings same as old settings */
    current.c_lflag &= ~ICANON; /* disable buffered i/o */
    if( echo )
    {
        current.c_lflag |= ECHO; /* set echo mode */
    }
    else
    {
        current.c_lflag &= ~ECHO; /* set no echo mode */
    }

    tcsetattr( 0, TCSANOW, &current ); /* use these new terminal i/o settings now */

    ch = getchar();

    tcsetattr( 0, TCSANOW, &old );
    return ch;
}

 /* Read 1 character without echo */
char _getch( void )
{
    return getch_( 0 );
}

/* Read 1 character with echo */
char getche( void )
{
    return getch_( 1 );
}

#endif //CROSS_GETCH_IMPLEMENTATION
#endif //__linux__

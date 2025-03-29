#include <stdio.h>
#include "field.h"

#ifdef _WIN32
#include <fcntl.h>
#include <conio.h>
#include <windows.h>

int getch()
{
    return _getch();
}
#endif

#ifdef __linux__
#include <termios.h>
#include <stdio.h>

static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

#endif

int main()
{

    field_s f;
    if( !fieldTryInit( &f, 12 ) )
    {
        my_puts("Error init!");
        return -1;
    }
    
    fieldDraw( &f );

    uint8_t cursor_pos = 0;
    cell_e chip = CELL_PLAYER_1;
    while(true)
    {
        int c = getch();
        if( c == 'a' )
        {
            if( cursor_pos > 0 )
                cursor_pos--;
        }
        else if( c == 'd' )
        {
            if( cursor_pos + 1 < f.width )
                cursor_pos++;
        }
        else if( c == 'q' || c == '\e')
        {
            break;
        }
        else if( c == ' ' )
        {
            step_e result = fieldPutChip( &f, cursor_pos, chip ); 
            if( result == STEP_WIN )
            {
                if( chip == CELL_PLAYER_1 )
                    my_puts("PLAYER 1 WIN!\r\n");
                else
                    my_puts("PLAYER 2 WIN!\r\n");
                break;
            }
            else if( result == STEP_NORMAL ) 
            {
                chip = (chip == CELL_PLAYER_1) ? CELL_PLAYER_2 : CELL_PLAYER_1;
            }
        }

        fieldDrawCursor( &f, cursor_pos, chip );
    }
    
    my_puts("Goodby!\r\n");
    return 0;
}

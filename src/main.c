#include <stdio.h>
#include "field.h"
#include "solver.h"
#include "ansi_codes.h"
#include <threads.h>

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
void initTermios( int echo )
{
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
}

/* Restore old terminal i/o settings */
void resetTermios( void )
{
    tcsetattr( 0, TCSANOW, &old );
}

/* Read 1 character - echo defines echo mode */
char getch_( int echo )
{
    char ch;
    initTermios( echo );
    ch = getchar();
    resetTermios();
    return ch;
}

 /* Read 1 character without echo */
char getch( void )
{
    return getch_( 0 );
}

/* Read 1 character with echo */
char getche( void )
{
    return getch_( 1 );
}

#endif

mtx_t mutex;


int thread_test_func( void *arg )
{
    const bool *isThreadWork = (const bool *) arg;
    const char *bubblesAnim[] = {
       ANSI_COLOR( ".", ANSI_HI_NORMAL, ANSI_WHITE ),
       ANSI_COLOR( "o", ANSI_HI_NORMAL, ANSI_YELLOW ),
       ANSI_COLOR( "O", ANSI_HI_NORMAL, ANSI_RED ),
       ANSI_COLOR( "@", ANSI_HI_BOLD, ANSI_RED ),
       ANSI_COLOR( "*", ANSI_NORMAL, ANSI_WHITE ),
       " " };
    const size_t bubblesAnimLength = sizeof( bubblesAnim ) / sizeof( bubblesAnim[0] );
    enum {
        NUMBER_OF_BUBBLES = FIELD_WIDTH * 2 + 2
    };
    uint8_t counters[NUMBER_OF_BUBBLES] = { 0 };
    while( *isThreadWork )
    {
        
        thrd_sleep( &( struct timespec ){ .tv_nsec = 60 * 1000 * 1000 }, NULL );
        for ( size_t i = 0; i < NUMBER_OF_BUBBLES; i++ )
        {
            if ( counters[i] < bubblesAnimLength - 1 )
                counters[i]++;
            if ( ( rand() & 0x07 ) == 0 )
                counters[i] = 0;
        }

        printf( "\e[s\e[7A" );
        for ( size_t i = 0; i < NUMBER_OF_BUBBLES; i++ )
        {
            printf( "%s", bubblesAnim[counters[i]] );
        }

        printf( "\e[u" );
        fflush( stdout );
    }
    return 0;
}

int main()
{
    field_s f = {0};

    // solverState_s state = CalculatePossibilities( &f );
  
    // printSolverState( &state );
    
    // return 0;
    printf( "\r\n" );
    fieldDraw( &f );

    // bool isThreadWork = true;
    // thrd_t t;
    // thrd_create( &t, thread_test_func, &isThreadWork );

    uint8_t cursor_pos = 0;
    cell_e chip = CELL_PLAYER_2;

    fieldDrawCursor( &f, cursor_pos, chip );
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
            if( cursor_pos + 1 < FIELD_WIDTH )
                cursor_pos++;
        }
        else if( c == 'q' || c == '\e')
        {
            break;
        }
        else if( c == ' ' )
        {
            if( fieldMoveAvailable( &f, cursor_pos ) )
            {
                uint8_t computer_move = CalculateBestMove( &f );
                turn_e turn = fieldTurn( &f, computer_move, cursor_pos, true );
                if( turn == TURN_NORMAL )
                {
                    if( fieldIsFull( &f ) )
                    {
                        printf( "DEAD HEAT\r\n" );
                        break;
                    }

                    chip = CELL_PLAYER_2;
                }
                else if( turn == TURN_WIN_PLAYER_1 )
                {
                    printf( "PLAYER 1 WIN!\r\n" );
                    break;
                }
                else if( turn == TURN_WIN_PLAYER_2 )
                {
                    printf( "PLAYER 2 WIN!\r\n" );
                    break;
                }
                else if( turn == TURN_DEAD_HEAT )
                {
                    printf( "DEAD HEAT\r\n" );
                    break;
                }
            }
        }

        fieldDrawCursor( &f, cursor_pos, chip );
    }
    
    // isThreadWork = false;
    // thrd_join( t, NULL );
    printf("Goodby!\r\n");
    return 0;
}

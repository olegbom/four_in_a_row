#include <stdio.h>
#include "field.h"
#include "solver.h"
#include "ansi_codes.h"
#include <threads.h>
#define CROSS_GETCH_IMPLEMENTATION
#include "cross_getch.h"

mtx_t mutex;


int thread_animation( void *arg )
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

typedef struct {
    field_s field;
    mtx_t mtx;
    cnd_t compute_condition;
    bool is_compute_required;
    cnd_t compute_result_condition;
    uint8_t compute_result;
    bool is_thread_work;
} fieldShareObject_s;

void fieldShareObject_init( fieldShareObject_s *s )
{
    s->field = (field_s){0};
    mtx_init( &s->mtx, mtx_plain );
    cnd_init( &s->compute_condition );
    s->is_compute_required = false;
    cnd_init( &s->compute_result_condition );
    s->compute_result = UINT8_MAX;
    s->is_thread_work = true;
}

int thread_compute_move( void *arg )
{
    bool is_off = false;
    fieldShareObject_s *share = (fieldShareObject_s *) arg;
    while(1)
    {
        mtx_lock( &share->mtx );
        while( !share->is_compute_required && share->is_thread_work)
        {
            // cnd_timedwait( &share->compute_condition, &share->mtx,
            //                &(struct timespec){.tv_nsec = 600 * 1000 * 1000} );
            cnd_wait( &share->compute_condition, &share->mtx );
        }
        
        is_off = !share->is_thread_work;

        field_s copy = share->field;
        mtx_unlock( &share->mtx );

        if( is_off )
        {
            break;
        }

        uint8_t computer_move = CalculateBestMove( &copy );
        mtx_lock( &share->mtx );
        share->compute_result = computer_move;
        share->is_compute_required = false;
        cnd_signal( &share->compute_result_condition );
        mtx_unlock( &share->mtx );
    }

    return 0;
}

int thread_ui( void *arg )
{
    fieldShareObject_s *share = (fieldShareObject_s *) arg;
     
    uint8_t cursor_pos = 0;
    cell_e chip = CELL_PLAYER_2;

    mtx_lock( &share->mtx );
    share->is_compute_required = true;
    cnd_signal( &share->compute_condition );
    mtx_unlock( &share->mtx );

    while(1)
    {
        mtx_lock( &share->mtx );
        fieldDrawCursor( &share->field, cursor_pos, chip );
        mtx_unlock( &share->mtx );
        int c = _getch();
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
            mtx_lock( &share->mtx );
            bool result = fieldMoveAvailable( &share->field, cursor_pos );
            mtx_unlock( &share->mtx );
            if( result )
            {
                mtx_lock( &share->mtx );
                if( share->compute_result == UINT8_MAX )
                {
                    cnd_wait( &share->compute_result_condition, &share->mtx );
                }

                turn_e turn = fieldTurn( &share->field, share->compute_result, cursor_pos, true );
                share->compute_result = UINT8_MAX;
                mtx_unlock( &share->mtx );
                if( turn == TURN_NORMAL )
                {
                    mtx_lock( &share->mtx );
                    bool isFull = fieldIsFull( &share->field );
                    mtx_unlock( &share->mtx );

                    if( isFull )
                    {
                        printf( "DEAD HEAT\r\n" );
                        break;
                    }

                    chip = CELL_PLAYER_2;
                    mtx_lock( &share->mtx );
                    share->is_compute_required = true;
                    cnd_signal( &share->compute_condition );
                    mtx_unlock( &share->mtx );
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
    }

    mtx_lock( &share->mtx );
    share->is_thread_work = false;
    cnd_signal( &share->compute_condition );
    mtx_unlock( &share->mtx );
    return 0;
}

int main()
{

    fieldShareObject_s share = {0};
    fieldShareObject_init( &share );

    // solverState_s state = CalculatePossibilities( &f );
    // printSolverState( &state );
    // return 0;

    printf( "\r\n" );
    fieldDraw( &share.field );

    thrd_t t_anim_handle;
    thrd_create( &t_anim_handle, thread_animation, &share.is_thread_work );

    thrd_t t_ui_handle;
    thrd_create( &t_ui_handle, thread_ui, &share );
    thrd_t t_calculate_move_handle;
    thrd_create( &t_calculate_move_handle, thread_compute_move, &share );


    thrd_join( t_anim_handle, NULL );
    thrd_join( t_ui_handle, NULL );
    thrd_join( t_calculate_move_handle, NULL );
    
    printf("Goodby!\r\n");
    return 0;
}

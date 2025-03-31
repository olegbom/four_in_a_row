#include "solver.h"
#include "field.h"
#include "stdio.h"

static step_e SolverMove( solverState_s *state, const field_s *field, uint8_t column,
                   cell_e chip );

solverState_s CalculatePossibilities( const field_s *field )
{
    solverState_s state = {0}; 
    for( uint8_t next_column = 0; next_column < FIELD_WIDTH; next_column++ )
    {
        SolverMove( &state, field, next_column, CELL_PLAYER_1 );
    }

    return state;
}


static step_e SolverMove( solverState_s *state, const field_s *field, uint8_t column,
                   cell_e chip )
{
    
    
    field_s copy = *field;
    step_e result = fieldPutChip( &copy, column, chip, false );
    switch( result )
    {
    case STEP_WIN:
        if( chip == CELL_PLAYER_1 )
        {
            state->number_of_winnings_of_player_1++; 
        }
        else 
        {
            state->number_of_winnings_of_player_2++; 
        }

        break;        
    case STEP_NORMAL:
        {
            uint8_t number_of_dead_heat = 0;
            cell_e next_chip = (cell_e)(CELL_PLAYER_1 + CELL_PLAYER_2 - chip);
            for( uint8_t next_column = 0; next_column < FIELD_WIDTH; next_column++ )
            {
                if( SolverMove( state, &copy, next_column, next_chip ) == STEP_NOT_AVAILABLE )
                {  
                    number_of_dead_heat++;
                } 
            }

            if( number_of_dead_heat == FIELD_WIDTH )
            {
                state->number_of_dead_heat++;
                if( state->number_of_dead_heat % 10000000 == 0 )
                {
                    printSolverState( state );
                }
            }
        }      
        break;
    case STEP_NOT_AVAILABLE:
    default:
        break; 
    }

    return result;    
} 

void printSolverState( const solverState_s *state )
{
    printf( "number_of_dead_heat %llu\r\n", state->number_of_dead_heat );
    printf( "number_of_winnings_of_player_1 %llu\r\n", state->number_of_winnings_of_player_1 );
    printf( "number_of_winnings_of_player_2 %llu\r\n", state->number_of_winnings_of_player_2 );
}

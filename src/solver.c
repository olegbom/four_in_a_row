#include "solver.h"
#include "field.h"
#include "stdio.h"

static turn_e SolverMove( solverState_s *state, const field_s *field, uint8_t column_p1,
                   uint8_t column_p2, uint8_t depth );

static double AttractivenessOfPath( const solverState_s *s ); 

uint8_t CalculateBestMove( const field_s *field )
{
    enum { MAX_DEPTH = 4 };
    solverState_s statuses[MAX_DEPTH][FIELD_WIDTH] = { 0 };
    for( uint8_t next_column_p1 = 0; next_column_p1 < FIELD_WIDTH; next_column_p1++ )
    {
        for( uint8_t next_column_p2 = 0; next_column_p2 < FIELD_WIDTH; next_column_p2++ )
        {
            for( uint8_t depth = 0; depth < MAX_DEPTH; depth++ )
            {
                SolverMove( &statuses[depth][next_column_p1], field, next_column_p1, next_column_p2, depth );
            }
        }
    }

    const double coeff[MAX_DEPTH] = {1000, 100, 10, 1};
    double bestValue = -1.0;
    uint8_t bestIndex = 0;
    for( uint8_t i = 0; i < FIELD_WIDTH; i++ )
    {
        double value = 0;
        for( uint8_t depth = 0; depth < MAX_DEPTH; depth++ )
        {
             value += AttractivenessOfPath( &statuses[depth][i] ) * coeff[depth];
        }

        if( bestValue < value )
        {
            bestIndex = i;
            bestValue = value;
        }
    }

    return bestIndex;
}

solverState_s CalculatePossibilities( const field_s *field )
{
    solverState_s state = {0};
    for( uint8_t next_column_p1 = 0; next_column_p1 < FIELD_WIDTH; next_column_p1++ )
        for( uint8_t next_column_p2 = 0; next_column_p2 < FIELD_WIDTH; next_column_p2++ )
        {
            SolverMove( &state, field, next_column_p1, next_column_p2, 255 );
        }

    return state;
}

static double AttractivenessOfPath( const solverState_s *s )
{
    return ( (double)s->number_of_winnings_of_player_1 - (double)s->number_of_winnings_of_player_2 ) /
           ( (double)s->number_of_dead_heat + (double)s->number_of_winnings_of_player_1 +
             (double)s->number_of_winnings_of_player_2 + (double)s->number_of_unsolved_paths );
}

static turn_e SolverMove( solverState_s *state, const field_s *field, uint8_t column_p1,
                   uint8_t column_p2, uint8_t depth )
{
    field_s copy = *field;
    turn_e result = fieldTurn( &copy, column_p1, column_p2, false );
    switch( result )
    {
    case TURN_DEAD_HEAT:
        state->number_of_dead_heat++;
        if( state->number_of_dead_heat % 10000000 == 0 )
        {
            printSolverState( state );
        }
        break;
    case TURN_NORMAL:
        {
            uint8_t number_of_dead_heat = 0;
            if( fieldIsFull( &copy ) )
            {
                state->number_of_dead_heat++;
                if( state->number_of_dead_heat % 10000000 == 0 )
                {
                    printSolverState( state );
                }
                break;
            }

            if( depth > 0 )
            {
                for( uint8_t next_column_p1 = 0; next_column_p1 < FIELD_WIDTH; next_column_p1++ )
                    for( uint8_t next_column_p2 = 0; next_column_p2 < FIELD_WIDTH; next_column_p2++ )
                    {
                        SolverMove( state, &copy, next_column_p1, next_column_p2, depth - 1 );
                    }
            }
            else
            {
                state->number_of_unsolved_paths++;
            }
        }      
        break;
    case TURN_WIN_PLAYER_1:
        state->number_of_winnings_of_player_1++;
        break;
    case TURN_WIN_PLAYER_2:
        state->number_of_winnings_of_player_2++;
        break;
    default:
        break; 
    }

    return result;    
} 

void printSolverState( const solverState_s *state )
{
    printf( "------------------------------\r\n" );
    printf( "number_of_unsolved_paths       %llu\r\n", state->number_of_unsolved_paths );
    printf( "number_of_dead_heat            %llu\r\n", state->number_of_dead_heat );
    printf( "number_of_winnings_of_player_1 %llu\r\n", state->number_of_winnings_of_player_1 );
    printf( "number_of_winnings_of_player_2 %llu\r\n", state->number_of_winnings_of_player_2 );
    printf( "------------------------------\r\n" );
}

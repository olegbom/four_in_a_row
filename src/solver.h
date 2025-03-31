#include "field.h"

typedef struct 
{
    uint64_t number_of_dead_heat;
    uint64_t number_of_winnings_of_player_1;  
    uint64_t number_of_winnings_of_player_2;  
    struct 
    {
        bool vertical[FIELD_WIDTH];
        bool fall_diag[FIELD_WIDTH - 3];
        bool rise_diag[FIELD_WIDTH - 3]; 
        bool horizontal[FIELD_WIDTH - 3][4];
    } hope;
} solverState_s;

solverState_s CalculatePossibilities( const field_s *field );
void printSolverState( const solverState_s *state );

#include "field.h"

typedef struct 
{
    uint64_t number_of_dead_heat;
    uint64_t number_of_winnings_of_player_1;  
    uint64_t number_of_winnings_of_player_2;  
    uint64_t number_of_unsolved_paths;
} solverState_s;

uint8_t CalculateBestMove( const field_s *field );
solverState_s CalculatePossibilities( const field_s *field );
void printSolverState( const solverState_s *state );

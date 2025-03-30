#include "field.h"

typedef struct 
{
    uint32_t number_of_dead_heat;
    uint32_t number_of_winnings_of_player_1;  
    uint32_t number_of_winnings_of_player_2;  
} solverState_s;

solverState_s CalculatePossibilities( const field_s *field );


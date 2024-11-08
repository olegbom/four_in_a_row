
/* -------------------------------- INCLUDES -------------------------------- */

#include "field.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* --------------------------------- DEFINES -------------------------------- */


/* --------------------------------- CONSTS --------------------------------- */

const char charset[4] = {' ', 'O', 'X', 'E'};

/* ----------------------- STATIC FUCNTIONS PROTOTYPES ---------------------- */

static cell_e fieldGetCell( const field_s *field, uint8_t row, uint8_t column );

/* -------------------------------- TYPEDEFS -------------------------------- */



bool fieldTryInit( field_s *field, uint8_t width )
{
    if( width > FIELD_MAX_WIDTH )
    {
        return false;
    }
    field->width = width;
    memset( field, 0, width );

    srand(time(NULL));
    for( uint32_t i = 0; i < field->width + 2; i++ )
    {
        field->cells[i] = (uint8_t) rand();        
    }
    
    return true;
}

void fieldDraw( const field_s *field )
{
    for( uint32_t j = 0; j < 4; j++ )
    {
        putchar('|');
        for( uint32_t i = 0; i < field->width; i++ )
        {
             putchar(charset[ fieldGetCell( field, j, i ) ]);
        }
        printf("|\r\n");
    }    

          
    for( uint32_t i = 0; i < field->width + 2; i++ )
    {
        putchar('-');
    }

    printf("\r\n");
}

/* ---------------------------- STATIC FUCNTIONS ---------------------------- */

static cell_e fieldGetCell( const field_s *field, uint8_t row, uint8_t column )
{
    if( !field || row >= 4 || column >= field->width )
    {
        return CELL_UNDEFINED;
    } 

    return (cell_e)( ( field->cells[column] >> (2*row) ) & 0x3 );
}

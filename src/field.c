
/* -------------------------------- INCLUDES -------------------------------- */

#include "field.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

/* --------------------------------- DEFINES -------------------------------- */


/* --------------------------------- CONSTS --------------------------------- */


// const char *charset[4] = {"  ", "\e[0;31m\ue0b6\ue0b4\e[0m", "\e[0;36m\ue0b6\ue0b4\e[0m", "  "};
const char *charset[4] = {"  ", "\e[0;31m()\e[0m", "\e[0;36m<>\e[0m", "  "};
/* ----------------------- STATIC FUCNTIONS PROTOTYPES ---------------------- */

static cell_e fieldGetCell( const field_s *field, uint8_t row, uint8_t column );

/* -------------------------------- TYPEDEFS -------------------------------- */



bool fieldTryInit( field_s *field, uint8_t width )
{
    setlocale(LC_ALL, "");
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
    printf(" ");       
    for( uint32_t i = 0; i < field->width/2; i++ )
    {
        printf("  ");
    }

    printf("\r\n");       
    
    for( uint32_t j = 0; j < 4; j++ )
    {
        printf("│");
        for( uint32_t i = 0; i < field->width; i++ )
        {
             printf("%s", charset[ fieldGetCell( field, j, i ) ]);
        }
        printf("│\r\n");
    }    

    printf("└");       
    for( uint32_t i = 0; i < field->width; i++ )
    {
        printf("──");
    }

    printf("┘\r\n");
    fieldDrawCursor( field, 0 );
}

void fieldDrawCursor( const field_s *field, uint8_t column )
{
    if( !field && column >= field->width )
        return;

    printf( "\e[s\e[6A " ); 
    for( int i = 0; i < column; i++ )
    {
        printf("  ");
    }
    
    // printf( "\ue0be\ue0bc" ); 
    printf( "\\/" ); 
    for( int i = column + 1; i < field->width; i++ )
    {
        printf("  ");
    }
    
    printf( "\e[u" );
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

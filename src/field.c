
/* -------------------------------- INCLUDES -------------------------------- */

#include "field.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* --------------------------------- DEFINES -------------------------------- */


/* --------------------------------- CONSTS --------------------------------- */

const char *charset[4] = {"  ", "\e[0;31m()\e[0m", "\e[0;36m<>\e[0m", "  "};

/* ----------------------- STATIC FUCNTIONS PROTOTYPES ---------------------- */

static cell_e fieldGetCell( const field_s *field, uint8_t row, uint8_t column );
static void fieldSetCell( field_s *field, uint8_t row, uint8_t column, cell_e value );

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
    // for( uint32_t i = 0; i < field->width + 2; i++ )
    // {
    //     field->cells[i] = (uint8_t) rand();        
    // }
    
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
        printf("|");
        for( uint32_t i = 0; i < field->width; i++ )
        {
             printf("%s", charset[ fieldGetCell( field, j, i ) ]);
        }
        printf("|\r\n");
    }    

    printf("\\");       
    for( uint32_t i = 0; i < field->width; i++ )
    {
        printf("--");
    }

    printf("/\r\n");
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

bool fieldPutChip( field_s *field, uint8_t column, cell_e chip )
{
    if( !field || column >= field->width ||
        (chip != CELL_PLAYER_1 && chip != CELL_PLAYER_2) )
    {
        return false;
    }
    
    for( uint8_t i = 0; i < 4; i++ )
    {
        uint8_t row = 3 - i;
        if( fieldGetCell( field, row, column) == CELL_EMPTY )
        {
            fieldSetCell( field, row, column, chip );
            printf( "\e[s\e[%uA\e[%uC", i + 2, column * 2 + 1 ); 
            printf( "%s", charset[ chip ] );
            printf( "\e[u" );

            //check horizontaly
            uint8_t score = 1;
            #define ADD_CHECK_SCORE score++; if( score == 4 ) return true;
            if( column >= 1 && fieldGetCell( field, row, column - 1 ) == chip )
            {
                ADD_CHECK_SCORE
                if( column >= 2 && fieldGetCell( field, row, column - 2 ) == chip )
                {
                    ADD_CHECK_SCORE
                    if( column >= 3 && fieldGetCell( field, row, column - 3 ) == chip )
                    {
                        ADD_CHECK_SCORE
                    }
                }
            }
            
            if( column + 1 <= field->width && fieldGetCell( field, row, column + 1 ) == chip )
            {
                ADD_CHECK_SCORE
                if( column + 2 <= field->width && fieldGetCell( field, row, column + 2 ) == chip )
                {
                    ADD_CHECK_SCORE
                    if( column + 3 <= field->width && fieldGetCell( field, row, column + 3 ) == chip )
                    {
                        ADD_CHECK_SCORE
                    }
                }
            }

            //check vertically
            if( field->cells[column] == ( chip | (chip << 2) | (chip << 4) | (chip << 6) ) )
            {
                return true;
            }
            
            break;
        }
    }
    
    return false;
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

static void fieldSetCell( field_s *field, uint8_t row, uint8_t column, cell_e value )
{
    if( !field || row >= 4 || column >= field->width )
    {
        return;
    } 

    field->cells[column] = ( field->cells[column] & ~(3 << (2*row)) ) | ( value << (2*row) );
}


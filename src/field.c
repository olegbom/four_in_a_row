
/* -------------------------------- INCLUDES -------------------------------- */

#include "field.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* --------------------------------- DEFINES -------------------------------- */


/* --------------------------------- CONSTS --------------------------------- */

const char *charset[4] = {"  ", "\e[0;31m()\e[0m", "\e[0;36m<>\e[0m", "  "};

/* ----------------------- STATIC FUCNTIONS PROTOTYPES ---------------------- */

static cell_e fieldGetCell( const field_s *field, uint8_t row, uint8_t column );
static void fieldSetCell( field_s *field, uint8_t row, uint8_t column, cell_e value );

/* -------------------------------- TYPEDEFS -------------------------------- */

int my_puts( const char *s )
{
    for( size_t i = 0; s[i]; ++i )
    {
        if( putchar( s[i] ) == EOF )
            return EOF;
    }
    return 0;
}

bool fieldTryInit( field_s *field, uint8_t width )
{
    if( width > FIELD_MAX_WIDTH )
    {
        return false;
    }
    field->width = width;
    memset( field, 0, width );
    return true;
}

void fieldDraw( const field_s *field )
{
    my_puts(" ");       
    for( uint32_t i = 0; i < field->width/2; i++ )
    {
        my_puts("  ");
    }

    my_puts("\r\n");       
    
    for( uint32_t j = 0; j < 4; j++ )
    {
        my_puts("|");
        for( uint32_t i = 0; i < field->width; i++ )
        {
             my_puts(charset[ fieldGetCell( field, j, i ) ]);
        }
        my_puts("|\r\n");
    }    

    my_puts("\\");       
    for( uint32_t i = 0; i < field->width; i++ )
    {
        my_puts("--");
    }

    my_puts("/\r\n");
    fieldDrawCursor( field, 0, CELL_PLAYER_1 );
}

void fieldDrawCursor( const field_s *field, uint8_t column, cell_e chip )
{
    if( !field || column >= field->width || 
        (chip != CELL_PLAYER_1 && chip != CELL_PLAYER_2) )
        return;

    my_puts( "\e[s\e[6A" ); 
    for( int i = 0; i < column; i++ )
    {
        my_puts("  ");
    }
    
    // my_puts( "\ue0be\ue0bc" ); 
    // my_puts( "\\/" );
    my_puts( "{" ); 
    my_puts( charset[ chip ] );
    my_puts( "}" ); 
    for( int i = column + 1; i < field->width; i++ )
    {
        my_puts("  ");
    }
    
    my_puts( "\e[u" );
}

step_e fieldPutChip( field_s *field, uint8_t column, cell_e chip )
{
    if( !field || column >= field->width ||
        (chip != CELL_PLAYER_1 && chip != CELL_PLAYER_2) )
    {
        return STEP_NOT_AVAILABLE;
    }
    
    for( uint8_t i = 0; i < 4; i++ )
    {
        uint8_t row = 3 - i;
        if( fieldGetCell( field, row, column) == CELL_EMPTY )
        {
            fieldSetCell( field, row, column, chip );
            my_puts( "\e[s" );
            for( uint32_t j = 0; j < i + 2; j++ )
            {
                my_puts( "\e[1A" );
            }

            for( uint32_t j = 0; j < column * 2 + 1; j++ )
            {
                my_puts( "\e[1C" );
            }

            my_puts( charset[ chip ] );
            my_puts( "\e[u" );

            //check horizontaly
            uint8_t score = 1;
            #define ADD_CHECK_SCORE score++; if( score == 4 ) return STEP_WIN;
            if( fieldGetCell( field, row, column - 1 ) == chip )
            {
                ADD_CHECK_SCORE
                if( fieldGetCell( field, row, column - 2 ) == chip )
                {
                    ADD_CHECK_SCORE
                    if( fieldGetCell( field, row, column - 3 ) == chip )
                    {
                        ADD_CHECK_SCORE
                    }
                }
            }
            
            if( fieldGetCell( field, row, column + 1 ) == chip )
            {
                ADD_CHECK_SCORE
                if( fieldGetCell( field, row, column + 2 ) == chip )
                {
                    ADD_CHECK_SCORE
                    if( fieldGetCell( field, row, column + 3 ) == chip )
                    {
                        ADD_CHECK_SCORE
                    }
                }
            }

            //check vertically
            if( field->cells[column] == ( chip | (chip << 2) | (chip << 4) | (chip << 6) ) )
            {
                return STEP_WIN;
            }
            
            return STEP_NORMAL;
        }
    }
    
    return STEP_NOT_AVAILABLE;
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


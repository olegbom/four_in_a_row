
/* -------------------------------- INCLUDES -------------------------------- */

#include "field.h"
#include <stdint.h>
#include <stdio.h>

/* --------------------------------- DEFINES -------------------------------- */


/* --------------------------------- CONSTS --------------------------------- */

static const char *Charset[4] = {"  ", "\e[0;31m()\e[0m", "\e[0;36m<>\e[0m", "  "};

static const uint8_t Masks[3] = {
    0, 
    ( CELL_PLAYER_1 | (CELL_PLAYER_1 << 2) | (CELL_PLAYER_1 << 4) | (CELL_PLAYER_1 << 6) ),
    ( CELL_PLAYER_2 | (CELL_PLAYER_2 << 2) | (CELL_PLAYER_2 << 4) | (CELL_PLAYER_2 << 6) )
};

/* ----------------------- STATIC FUCNTIONS PROTOTYPES ---------------------- */

static cell_e fieldGetCell( const field_s *field, uint8_t row, uint8_t column );
static void fieldSetCell( field_s *field, uint8_t row, uint8_t column, cell_e value );
static void printChip( uint8_t row, uint8_t column, cell_e chip );
static step_e checkScore( const field_s *field, uint8_t row, uint8_t column, cell_e chip );

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

void fieldDraw( const field_s *field )
{
    my_puts(" ");       
    for( uint32_t i = 0; i < FIELD_WIDTH/2; i++ )
    {
        my_puts("  ");
    }

    my_puts("\r\n");       
    
    for( uint32_t j = 0; j < 4; j++ )
    {
        my_puts("|");
        for( uint32_t i = 0; i < FIELD_WIDTH; i++ )
        {
             my_puts(Charset[ fieldGetCell( field, j, i ) ]);
        }
        my_puts("|\r\n");
    }    

    my_puts("\\");       
    for( uint32_t i = 0; i < FIELD_WIDTH; i++ )
    {
        my_puts("--");
    }

    my_puts("/\r\n");
    fieldDrawCursor( field, 0, CELL_PLAYER_1 );
}

void fieldDrawCursor( const field_s *field, uint8_t column, cell_e chip )
{
    if( !field || column >= FIELD_WIDTH || 
        (chip != CELL_PLAYER_1 && chip != CELL_PLAYER_2) )
        return;

    my_puts( "\e[s\e[6A" ); 
    for( int i = 0; i < column; i++ )
    {
        my_puts("  ");
    }
    
    my_puts( "{" ); 
    my_puts( Charset[ chip ] );
    my_puts( "}" ); 
    for( int i = column + 1; i < FIELD_WIDTH; i++ )
    {
        my_puts("  ");
    }
    
    my_puts( "\e[u" );
}

step_e fieldPutChip( field_s *field, uint8_t column, cell_e chip, bool is_print )
{
    // if( !field || column >= FIELD_WIDTH ||
    //     (chip != CELL_PLAYER_1 && chip != CELL_PLAYER_2) )
    // {
    //     return STEP_NOT_AVAILABLE;
    // }

    for( uint8_t i = 0; i < 4; i++ )
    {
        uint8_t row = 3 - i;
        if( fieldGetCell( field, row, column ) == CELL_EMPTY )
        {
            fieldSetCell( field, row, column, chip );
            if( is_print )
            {
                printChip( row, column, chip );
            }
            
            return checkScore( field, row, column, chip );
        }
    }
    
    return STEP_NOT_AVAILABLE;
}

bool fieldIsFull( const field_s *field )
{
    for( uint8_t col = 0; col < FIELD_WIDTH; col++ )
    {
        if( fieldGetCell( field, 0, col ) == CELL_EMPTY )
        {
            return false;
        }
    }
    return true;
}
/* ---------------------------- STATIC FUCNTIONS ---------------------------- */

static cell_e fieldGetCell( const field_s *field, uint8_t row, uint8_t column )
{
    // if( row >= 4 || column >= FIELD_WIDTH )
    // {
    //     return CELL_UNDEFINED;
    // } 

    return (cell_e)( ( field->cells[column] >> (2*row) ) & 0x3 );
}

static void fieldSetCell( field_s *field, uint8_t row, uint8_t column, cell_e value )
{
    // if( !field || row >= 4 || column >= FIELD_WIDTH )
    // {
    //     return;
    // } 

    field->cells[column] = ( field->cells[column] & ~(3 << (2*row)) ) | ( value << (2*row) );
}

static void printChip( uint8_t row, uint8_t column, cell_e chip )
{
    printf( "\e[s\e[%dA\e[%dC%s\e[u", 5 - row, column * 2 + 1, Charset[ chip ] );
}

static step_e checkScore( const field_s *field, uint8_t row, uint8_t column, cell_e chip )
{
    //check horizontaly
    uint8_t score = 1;
    #define ADD_CHECK_SCORE(i,j) \
        if( (uint8_t)(column + j) >= FIELD_WIDTH || fieldGetCell( field, row + i, column + j) != chip )\
            break;

    do
    {
        ADD_CHECK_SCORE(0,-1);
        score++; 
        ADD_CHECK_SCORE(0,-2);
        score++; 
        ADD_CHECK_SCORE(0,-3);
        return STEP_WIN;
    }while(0);

    do
    {
        ADD_CHECK_SCORE(0, 1);
        score++; 
        if( score == 4 ) return STEP_WIN;
        ADD_CHECK_SCORE(0, 2);
        score++; 
        if( score == 4 ) return STEP_WIN;
        ADD_CHECK_SCORE(0, 3);
        return STEP_WIN;
    }while(0);   
    
    //check vertically
    if( field->cells[column] == Masks[chip] )
    {
        return STEP_WIN;
    }

    //check diagonally
    if( (uint8_t)(column - row) < FIELD_WIDTH &&
        (uint8_t)(column - row + 3) < FIELD_WIDTH &&
        fieldGetCell( field, 0, column - row ) == chip &&
        fieldGetCell( field, 1, column - row + 1 ) == chip &&
        fieldGetCell( field, 2, column - row + 2 ) == chip &&
        fieldGetCell( field, 3, column - row + 3 ) == chip )
        return STEP_WIN;
            
    if( (uint8_t)(column + row) < FIELD_WIDTH &&
        (uint8_t)(column + row - 3) < FIELD_WIDTH &&
        fieldGetCell( field, 0, column + row ) == chip &&
        fieldGetCell( field, 1, column + row - 1 ) == chip &&
        fieldGetCell( field, 2, column + row - 2 ) == chip &&
        fieldGetCell( field, 3, column + row - 3 ) == chip )
        return STEP_WIN;
            
    return STEP_NORMAL;
}

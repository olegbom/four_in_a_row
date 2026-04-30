#include <stdio.h>

#include "ansi_codes.h"
#include "field.h"
#include "field3d.h"
#include "cross_getch.h"

typedef enum {
    BORDER_TYPE_CORNER_TOP_LEFT = 0,
    BORDER_TYPE_CORNER_TOP_RIGHT,
    BORDER_TYPE_CORNER_BOTTOM_LEFT,
    BORDER_TYPE_CORNER_BOTTOM_RIGHT,
    BORDER_TYPE_HORIZONTAL_BAR,
    BORDER_TYPE_VERTICAL_BAR,
    BORDER_TYPE_CENTER_LEFT,
    BORDER_TYPE_CENTER_RIGHT,
    BORDER_TYPE_SPACE,
    NUMBER_OF_BORDER_TYPES,
} borderType_e;

typedef struct {
    const char *s[NUMBER_OF_BORDER_TYPES];
} border_s;

#define CELL_WIDTH  ((uint32_t)14)
#define CELL_HEIGHT ((uint32_t)7)

const uint8_t cell_indent[CELL_WIDTH * CELL_HEIGHT] = {
    48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48,
    48, 48, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 48, 48,
    48, 48, 32, 32, 16, 16, 16, 16, 16, 16, 32, 32, 48, 48,
    48, 48, 32, 32, 16, 16,  0,  0, 16, 16, 32, 32, 48, 48,
    48, 48, 32, 32, 16, 16, 16, 16, 16, 16, 32, 32, 48, 48,
    48, 48, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 48, 48, 
    48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48,
};

#define TL BORDER_TYPE_CORNER_TOP_LEFT,
#define TR BORDER_TYPE_CORNER_TOP_RIGHT,
#define BL BORDER_TYPE_CORNER_BOTTOM_LEFT,
#define BR BORDER_TYPE_CORNER_BOTTOM_RIGHT,
#define HB BORDER_TYPE_HORIZONTAL_BAR,
#define VB BORDER_TYPE_VERTICAL_BAR,
#define CL BORDER_TYPE_CENTER_LEFT,
#define CR BORDER_TYPE_CENTER_RIGHT,
#define SP BORDER_TYPE_SPACE,

const borderType_e cell_border_type[CELL_WIDTH * CELL_HEIGHT] = {
    TL HB HB HB HB HB HB HB HB HB HB HB HB TR
    VB SP TL HB HB HB HB HB HB HB HB TR SP VB    
    VB SP VB SP TL HB HB HB HB TR SP VB SP VB
    VB SP VB SP VB SP CL CR SP VB SP VB SP VB
    VB SP VB SP BL HB HB HB HB BR SP VB SP VB
    VB SP BL HB HB HB HB HB HB HB HB BR SP VB
    BL HB HB HB HB HB HB HB HB HB HB HB HB BR
};

#undef TL
#undef TR
#undef BL
#undef BR
#undef HB
#undef VB
#undef CE
#undef SP


const border_s EmptyBorders = {
    .s = {
        [BORDER_TYPE_CORNER_TOP_LEFT]     = ANSI_COLOR(" ", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_CORNER_TOP_RIGHT]    = ANSI_COLOR(" ", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_CORNER_BOTTOM_LEFT]  = ANSI_COLOR(" ", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_CORNER_BOTTOM_RIGHT] = ANSI_COLOR(" ", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_HORIZONTAL_BAR]      = ANSI_COLOR(" ", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_VERTICAL_BAR]        = ANSI_COLOR(" ", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_CENTER_LEFT]         = ANSI_COLOR(" ", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_CENTER_RIGHT]        = ANSI_COLOR(" ", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_SPACE]               = ANSI_COLOR(" ", ANSI_NORMAL, ANSI_YELLOW),
    }
};

const border_s Player1Borders = {
    .s = {
        [BORDER_TYPE_CORNER_TOP_LEFT]     = ANSI_COLOR("┏", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_CORNER_TOP_RIGHT]    = ANSI_COLOR("┓", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_CORNER_BOTTOM_LEFT]  = ANSI_COLOR("┗", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_CORNER_BOTTOM_RIGHT] = ANSI_COLOR("┛", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_HORIZONTAL_BAR]      = ANSI_COLOR("━", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_VERTICAL_BAR]        = ANSI_COLOR("┃", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_CENTER_LEFT]         = ANSI_COLOR("[", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_CENTER_RIGHT]        = ANSI_COLOR("]", ANSI_NORMAL, ANSI_YELLOW),
        [BORDER_TYPE_SPACE]               = ANSI_COLOR(" ", ANSI_NORMAL, ANSI_YELLOW),
    }
};

const border_s Player2Borders = {
    .s = {
        [BORDER_TYPE_CORNER_TOP_LEFT]     = ANSI_COLOR("╭", ANSI_NORMAL, ANSI_CYAN),
        [BORDER_TYPE_CORNER_TOP_RIGHT]    = ANSI_COLOR("╮", ANSI_NORMAL, ANSI_CYAN),
        [BORDER_TYPE_CORNER_BOTTOM_LEFT]  = ANSI_COLOR("╰", ANSI_NORMAL, ANSI_CYAN),
        [BORDER_TYPE_CORNER_BOTTOM_RIGHT] = ANSI_COLOR("╯", ANSI_NORMAL, ANSI_CYAN),
        [BORDER_TYPE_HORIZONTAL_BAR]      = ANSI_COLOR("─", ANSI_NORMAL, ANSI_CYAN),
        [BORDER_TYPE_VERTICAL_BAR]        = ANSI_COLOR("│", ANSI_NORMAL, ANSI_CYAN),
        [BORDER_TYPE_CENTER_LEFT]         = ANSI_COLOR("(", ANSI_NORMAL, ANSI_CYAN),
        [BORDER_TYPE_CENTER_RIGHT]        = ANSI_COLOR(")", ANSI_NORMAL, ANSI_CYAN),
        [BORDER_TYPE_SPACE]               = ANSI_COLOR(" ", ANSI_NORMAL, ANSI_CYAN),
    }
};

const border_s BothBorders = {
    .s = {
      [BORDER_TYPE_CORNER_TOP_LEFT]     = ANSI_COLOR("╔", ANSI_NORMAL, ANSI_RED),
      [BORDER_TYPE_CORNER_TOP_RIGHT]    = ANSI_COLOR("╗", ANSI_NORMAL, ANSI_RED),
      [BORDER_TYPE_CORNER_BOTTOM_LEFT]  = ANSI_COLOR("╚", ANSI_NORMAL, ANSI_RED),
      [BORDER_TYPE_CORNER_BOTTOM_RIGHT] = ANSI_COLOR("╝", ANSI_NORMAL, ANSI_RED),
      [BORDER_TYPE_HORIZONTAL_BAR]      = ANSI_COLOR("═", ANSI_NORMAL, ANSI_RED),
      [BORDER_TYPE_VERTICAL_BAR]        = ANSI_COLOR("║", ANSI_NORMAL, ANSI_RED),
      [BORDER_TYPE_CENTER_LEFT]         = ANSI_COLOR("#", ANSI_NORMAL, ANSI_RED),
      [BORDER_TYPE_CENTER_RIGHT]        = ANSI_COLOR("#", ANSI_NORMAL, ANSI_RED),
      [BORDER_TYPE_SPACE]               = ANSI_COLOR(" ", ANSI_NORMAL, ANSI_RED),
    }
};

const border_s *Borders[4] = {
    [CELL_EMPTY]    = &EmptyBorders,
    [CELL_PLAYER_1] = &Player1Borders,
    [CELL_PLAYER_2] = &Player2Borders,
    [CELL_BOTH]     = &BothBorders,
};

#define NUMBER_OF_WIN_MASKS  (sizeof(win_masks)/sizeof(win_masks[0]))
const uint64_t win_masks[] = {
    //x
    //FEDCBA9876543210
    0x000000000000000F, 
    0x00000000000000F0, 
    0x0000000000000F00, 
    0x000000000000F000, 
    //FEDCBA9876543210
    0x00000000000F0000, 
    0x0000000000F00000, 
    0x000000000F000000, 
    0x00000000F0000000, 
    //FEDCBA9876543210
    0x0000000F00000000, 
    0x000000F000000000, 
    0x00000F0000000000, 
    0x0000F00000000000, 
    //FEDCBA9876543210
    0x000F000000000000, 
    0x00F0000000000000, 
    0x0F00000000000000, 
    0xF000000000000000,
    //y
    //FEDCBA9876543210
    0x0000000000001111,
    0x0000000000002222,
    0x0000000000004444,
    0x0000000000008888,
    //FEDCBA9876543210
    0x0000000011110000,
    0x0000000022220000,
    0x0000000044440000,
    0x0000000088880000,
    //FEDCBA9876543210
    0x0000111100000000,
    0x0000222200000000,
    0x0000444400000000,
    0x0000888800000000,
    //FEDCBA9876543210
    0x1111000000000000,
    0x2222000000000000,
    0x4444000000000000,
    0x8888000000000000,
    //z
    //FEDCBA9876543210
    0x0001000100010001,
    0x0002000200020002,
    0x0004000400040004,
    0x0008000800080008,
    //FEDCBA9876543210
    0x0010001000100010,
    0x0020002000200020,
    0x0040004000400040,
    0x0080008000800080,
    //FEDCBA9876543210
    0x0100010001000100,
    0x0200020002000200,
    0x0400040004000400,
    0x0800080008000800,
    //FEDCBA9876543210
    0x1000100010001000,
    0x2000200020002000,
    0x4000400040004000,
    0x8000800080008000,
    //+x+y
    //FEDCBA9876543210
    0x0000000000008421,
    0x0000000084210000,
    0x0000842100000000,
    0x8421000000000000,
    //-x+y
    //FEDCBA9876543210
    0x0000000000001248,
    0x0000000012480000,
    0x0000124800000000,
    0x1248000000000000,
    //+x+z
    //FEDCBA9876543210
    0x0008000400020001,
    0x0080004000200010,
    0x0800040002000100,
    0x8000400020001000,
    //+x-z
    //FEDCBA9876543210
    0x0001000200040008,
    0x0010002000400080,
    0x0100020004000800,
    0x1000200040008000,
    //+y+z
    //FEDCBA9876543210
    0x1000010000100001,
    0x2000020000200002,
    0x4000040000400004,
    0x8000080000800008,
    //+y-z
    //FEDCBA9876543210
    0x0001001001001000,
    0x0002002002002000,
    0x0004004004004000,
    0x0008008008008000,
    //+x+y+z
    //FEDCBA9876543210
    0x8000040000200001,
    //+x+y-z
    //FEDCBA9876543210
    0x0001002004008000,
    //+x-y+z
    //FEDCBA9876543210
    0x1000020000400008,
    //+x-y-z
    //FEDCBA9876543210
    0x0008004002001000,
};

typedef struct
{
    uint64_t m[7];
    size_t  size;
} cellWinMasks_s;

static cellWinMasks_s cellWinMasks[64] = {0};

static void calculateCellWinMasksCounts()
{
    for(size_t i = 0; i < sizeof(cellWinMasks)/sizeof(cellWinMasks[0]); i++)
    {
        cellWinMasks[i] = (cellWinMasks_s) {0};
        uint64_t mask = 1ull << i;
        for(size_t j = 0; j < NUMBER_OF_WIN_MASKS; j++)
        {
            if( (mask & win_masks[j]) != 0 )
            {
                cellWinMasks[i].m[cellWinMasks[i].size] = win_masks[j];
                cellWinMasks[i].size++;
            }
        }
        printf("%zu", cellWinMasks[i].size);
    }

}

const uint64_t moves_masks[16] = {
    //FEDCBA9876543210
    0x0001000100010001,
    0x0002000200020002,
    0x0004000400040004,
    0x0008000800080008,
    //FEDCBA9876543210
    0x0010001000100010,
    0x0020002000200020,
    0x0040004000400040,
    0x0080008000800080,
    //FEDCBA9876543210
    0x0100010001000100,
    0x0200020002000200,
    0x0400040004000400,
    0x0800080008000800,
    //FEDCBA9876543210
    0x1000100010001000,
    0x2000200020002000,
    0x4000400040004000,
    0x8000800080008000,
};

bool field3dMoveAvailable( field3d_s f, uint8_t column, uint8_t row )
{
    if( column > 3 && row > 3 )
        return false;

    size_t i = column + row * 4 + 48;
    uint64_t isNotEmpty = f.a | f.b;
    return (isNotEmpty & (1ull << i)) != 0;
}

void field3dTestMasksVisual()
{
    enum {
        FIELD_3D_PRINT = 0,
        CELL_WIN_MASKS_COUNTS,
    } state = FIELD_3D_PRINT;

    size_t mask_index = NUMBER_OF_WIN_MASKS - 1;
    while(1)
    {
        printf( "\e[H" );
        switch (state)
        {
        case FIELD_3D_PRINT:
            field3dPrint( ( field3d_s ){ .a = win_masks[mask_index], .b = 0 } );
            break;
        case CELL_WIN_MASKS_COUNTS:
            calculateCellWinMasksCounts();
            break;
        }

        int c = _getch();
        if( c == 'a' )
        {
            if( mask_index > 0 )
                mask_index--;
        }
        else if( c == 'd' )
        {
            if( mask_index + 1 < NUMBER_OF_WIN_MASKS )
                mask_index++;
        }
        else if( c == 'q' || c == '\e')
        {
            break;
        }
        else if (c == '1')
        {
            state = FIELD_3D_PRINT;
        }
        else if (c == '2')
        {
            state = CELL_WIN_MASKS_COUNTS;
        }
    }

    
}


void field3dPrint(field3d_s f)
{
    printf("  FEDCBA9876543210\r\n");
    printf("0x%016llX\r\n", f.a);
    for( size_t c_column = 0; c_column < 4; c_column++ )
    {
        printf( "|              " );
    }

    printf( "|\r\n" );
    for(size_t c_row = 0; c_row < 4; c_row++ )
    {
        for( size_t row = 0; row < CELL_HEIGHT; row++ )
        {
            printf( row == CELL_HEIGHT - 1 ? "_" : " " );
            for( size_t c_column = 0; c_column < 4; c_column++ )
            {
                for( size_t column = 0; column < CELL_WIDTH; column++ )
                {
                    size_t cell_index = row * CELL_WIDTH + column;
                    borderType_e border = cell_border_type[cell_index];
                    uint64_t bit_index = cell_indent[cell_index] + c_row * 4 + c_column;
                    uint64_t mask = 1ull << bit_index;
                    bool a = ( f.a & mask ) != 0;
                    bool b = ( f.b & mask ) != 0;
                    cell_e type = a ? ( b ? CELL_BOTH : CELL_PLAYER_1 ) : ( b ? CELL_PLAYER_2 : CELL_EMPTY );
                    printf( "%s", Borders[type]->s[border] );
                }
                printf( row == CELL_HEIGHT - 1 ? "." : " " );
            }

            printf( "\r\n" );
        }
    }
}

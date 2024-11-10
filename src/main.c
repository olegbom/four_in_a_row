#include <stdio.h>
#include "field.h"
#include <conio.h>

#include <fcntl.h>
#include <io.h>
#include <windows.h>

__attribute__((constructor))
void
winsane_init(void)
{
    _setmode(0, _O_BINARY);
    _setmode(1, _O_BINARY);
    SetConsoleCP(CP_UTF8);  // maybe will work someday
    SetConsoleOutputCP(CP_UTF8);
}


int main()
{
    winsane_init();

    field_s f;
    if( !fieldTryInit( &f, 12 ) )
    {
        printf("Error init!");
        return -1;
    }
    
    fieldDraw( &f );

    uint8_t cursor_pos = 0;
    while(true)
    {
        int c = _getch();
        if( c == 'a' )
        {
            if( cursor_pos > 0 )
                cursor_pos--;
        }
        else if( c == 'd' )
        {
            if( cursor_pos + 1 < f.width )
                cursor_pos++;
        }
        else if( c == 'q' )
        {
            break;
        }

        fieldDrawCursor( &f, cursor_pos );
    }
    
    printf("Goodby!");
    return 0;
}

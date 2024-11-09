#include <stdio.h>
#include "field.h"


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
    
    printf("Goodby!");
    return 0;
}

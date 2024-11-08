#include <stdio.h>
#include "field.h"

int main()
{
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

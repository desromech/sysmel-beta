#include <stdio.h>

// On windows, some std library functions are actually inlined.
int ensureFunctionsAreImported()
{
    printf(" ");
    return 0;
}
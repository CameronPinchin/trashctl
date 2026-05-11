#include "../include/safe_dd.h"
#include <stdio.h>
#include <stdlib.h>

void init_message()
{
    fputs("Usage:  safe-dd [OPERANDS]...\n or:   safe-dd OPTIONS\n", stdout);
    fputs("Safely copy a file, converting and formatting according to the operands.\n\n", stdout);
    fputs("  bs=BYTES          read and write up to BYTES bytes at a time (default: 512).", stdout);
    fputs("  cbs=BYTES         convert BYTES bytes at a time", stdout);

}

int main(int argc, char** argv)
{
    init_message();
    return EXIT_SUCCESS;
}

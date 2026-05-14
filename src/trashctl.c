#include "../include/trashctl.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    int err;
    if((err = initialize(argc, argv)) == 1){
        return EXIT_FAILURE;
    }

}

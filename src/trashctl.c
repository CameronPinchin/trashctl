#include "../include/trashctl.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief The main function for the program.
 *
 * Simply delegates work to initialize() by passing command line arguments.
 *
 * @param[in] argc An integer representation of the number of arguments passed to trashctl.
 * @param[in] argv An array representation of the arguments passed to trashctl.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
int main(int argc, char** argv)
{
    int err;
    if((err = initialize(argc, argv)) == 1){
        return EXIT_FAILURE;
    }
    return 0;
}

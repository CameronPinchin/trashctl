#include "../include/trashctl.h"
/* intended as a location for generalized, shared functions to be reused across multiple files */


void construct_path(char *buf, const char* trashdir, const char* file, const char* opt_srcprefix, int size)
{
    if(trashdir != NULL){
        strlcpy(buf, trashdir, size);
    }

    if(file != NULL){
        strlcat(buf, file, size);
    }

    if(opt_srcprefix != NULL){
        strlcat(buf, opt_srcprefix, size);
    }
}

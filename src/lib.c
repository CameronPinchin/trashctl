#include "../include/trashctl.h"
/* intended as a location for generalized, shared functions to be reused across multiple files */

/**
 * @brief Shared function for path construction.
 *
 * Constructs filepaths based on optional inputs.
 *
 * @param[in] buf The destination buffer chars should be copied into.
 * @param[in] trashdir An optional source buffer representing the trashdir path.
 * @param[in] file An optional source buffer representing filenames.
 * @param[in] opt_srcprefix An optional source buffer representing file prefixes.
 * @param[in] size A mandatory size option for safe string manipulation.
 */
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

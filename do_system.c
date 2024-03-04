#include "systemcalls.h"
#include <stdlib.h>

bool do_system(const char *cmd)
{
    int result = system(cmd);

    if (result == 0) {
        return true;  // Success
    } else {
        return false; // Failure
    }
}

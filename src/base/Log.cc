#include "src/base/Log.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "src/base/TimeStamp.h"
#include "src/base/TimeZone.h"

thread_local char t_errnoBuf[512];
thread_local char t_time[64];
thread_local time_t t_lastSecond;

const char *strerror_tl(int savedErrno){
    return strerror_r(savedErrno,t_errnoBuf,sizeof(t_errnoBuf));
}










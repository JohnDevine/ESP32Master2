#include "jd_global.h"

// Uncomment one of the below to only put only messages up to a certain level in the compiled code
// (You still need to turn them on with setDebug(someLevel) to see them)
// #define EZTIME_MAX_DEBUGLEVEL_NONE
// #define EZTIME_MAX_DEBUGLEVEL_ERROR
// #define EZTIME_MAX_DEBUGLEVEL_INFO
#include <ezTime.h>

// https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
#define MY_TIMEZONE "Asia/Bangkok"
#define NTPSERVER_QUERY_INTERVAL_SECONDS 60*60

// if ARDUINOTRACE_ENABLE = true output to the console the current time in many forms.
void testTime();

// Initialise the NTP server and set the timezone
// Select the debug level from:
// 	NONE,
//	ERROR,
//	INFO,
//	DEBUG
void initNTPsetTimezone(char *in_tz, ezDebugLevel_t debug_level);
void jd_getCurrentTime(char *out_time, int maxlen, String format);
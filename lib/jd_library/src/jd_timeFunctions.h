#include "jd_global.h"
#include <ezTime.h>
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

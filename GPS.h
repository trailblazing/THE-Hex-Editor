/******************************************************************************
*
*
*             GPS.H: Main GPS Include file
*
*
*
*       This is the main preprocessor file for the GPS library.  It
contains   *
*   definitions of the GPS data structures, external references to the GPS
*
*   library's error variables,  user turns the GPS off.
*
*
*
******************************************************************************/

#include <time.h>

// don't pad structures in gps.h
#pragma pack(1)

#define MAX_MSG_SIZE    1500


typedef struct
{
    double true_heading;
    double speed;
    double latitude, longitude;
    double altitude;
//    int terrain;        // terrain settings 0=clear,
    // 1=interrupted, 2=obstructed
//   int datum;          // Map datum 1 - 47
//   int dist_sp_units;  // Distance/speed units:  0=nautical miles/knots
    // 1 = km/kph,  2 = miles/mph
//   int alt_units;      // Altitude units 0=Feet 1=Meter
//   int mag_var;        // Magnetic Variation:
    // 0 = user entered, 1 = Auto derived
    struct
    {
        unsigned havecomm : 1;
        unsigned terrain : 2;
        unsigned datum   : 6;
        unsigned dist_sp_units : 2;
        unsigned alt_units : 2;
        unsigned mag_var : 2;
        unsigned nav : 1;
    } flags;

    char hardware_mode;

    time_t timestamp;
    struct
    {
        unsigned corrected : 1;
        unsigned filler    : 15;
    } flags2;

//    LOG_SAT satellites[5];
    unsigned char satellites[5];

    char filler[2];
} GPS_DATA;

#if defined( PSII_INSTRUMENT)

enum MSG_TYPE {POS_MSG, RCV_MSG, COURSE_MSG, UNITS_MSG};

typedef struct course_struct
{
    time_t atime;        // acquisition time
    int changed;
    double true_heading;     // True heading in degrees
    double speed;      // speed in km/hr
} COURSE, *COURSEPTR;

typedef struct pos_struct
{
    time_t atime;
    int changed;
    double bin_lat, bin_lon;		// lat/lon in 10^-7 degrees
    double bin_alt;		// altitude in .01 meters/feet
    char utc_time[7];    // utc of position in hhmmss
    char lat[8];         // GPS latitude xxxx.xx
    char lat_hem[2];     // latitude hemispheres N/S
    char lon[8];         // GPS longitude xxxx.xx
    char lon_hem[2];     // longitude hemispheres E/W
    int  quality;        // quality indicator -  0 = GPS unavailable
    //                      1 = GPS available
    int  sat_cnt;        // number of sats being used
    char hdop[3];
    char ant_height[7];  // Antenna height uxx
    char ant_units[2];   // Units of antenna height  M=meters F=Feet
    char geo_height[4];  // Geoidal height uxx
    char geo_units[2];   // Units of geoidal height  M\n=meters F\=Feet
    time_t timestamp;
} POS, *POSPTR;

typedef struct
{
    time_t atime;
    long lat, lon, alt;
    long timetag;
} NATIVE_POS;

typedef struct rcv_struct
{
    time_t atime;       // acquisition time
    int changed;
    float version;      // software version xx.x
    int customer;       // Customer number 0 - 99
    int power;          // Battery power 0=ok 1=low
    int osc;            // Oscillator 0 = ok 1=out of tune
    int sq;             // signal quality
    int gq;             // geometric quality
    int nav;            // nav solution 0=continuous, 1=interrupted
    int alm;            // Almanac data: 0=ok, 1=no almanac data,
    //               2= data is old
    int mem;            // Memory: 0=ok 1=lost memory data
    int status;         // OEM unit status:
//                           0=initialization mode    1=idle
//                           2=status 3=almanac       4=ephemeris
//                           5=acquisition            6=position
//                           7=navigation
} RCV, *RCVPTR;

typedef struct units_type
{
    int changed;
    int terrain;        // terrain settings 0=clear,
    // 1=interrupted, 2=obstructed
    int datum;          // Map datum 1 - 47
    int dist_sp_units;  // Distance/speed units:  0=nautical miles/knots
    // 1 = km/kph,  2 = miles/mph
    int alt_units;      // Altitude units 0=Feet 1=Meter
    int mag_var;        // Magnetic Variation:
    // 0 = user entered, 1 = Auto derived
} UNITS, *UNITS_PTR;

typedef struct msg
{
    enum MSG_TYPE msg_type;
    time_t timestamp;
    union
    {
        POS pos;
        RCV rcv;
        COURSE course;
        UNITS units;
    } data;
} _MSG, *MSGPTR;

typedef struct satstat_struct
{
    time_t atime;
    int sat[4];
    int stat[4];
} SATSTAT, *SATSTAT_PTR;

typedef struct satusage_struct
{
    int sat[5];
    boolean constellation[5];
    byte tracking[5];
    boolean ephemeris[5];
} SATUSAGE, *SATUSAGE_PTR;

typedef struct msg_type
{
    char header[10];
    int size;
    int (*parse_function)(byte *);
    char *data;
} MSG_DESC;

typedef struct
{
    unsigned prn : 5;       // Satellite PRN number, 0-24
    unsigned nav : 1;       // Is satellite used in navigation solution
    unsigned status : 2;    // 0 = ACQ, 1 = searching, 2 = 0-3 SQ, 3 = 4-9 SQ
} LOG_SAT;

typedef struct
{
    char log_path[65];
    char log_file[9];
    char log_msg[81];
    char header_comment[81];
    float latitude, longitude, altitude;
    int gmt_diff;
    int dist_units, alt_units;
    int dimension;
    int feature;
    int datum;
} CONFIG, *CONFIG_PTR;

typedef struct
{
    int changed;
    int mode;
} MODE;

// and return to default packing alignment
#pragma pack()

// #include "gpserrno.h"

extern int  gps_errno;
extern char gps_errstr[81];
extern float gps_version;

// time of last message from GPS hardware, or -1 if none
extern clock_t receive_time;

#ifdef __cplusplus
extern "C" {
#endif

//   Message function prototypes and externals.
// byte calculate_checksum(byte *msg, int length);
MSG_DESC *msg_Get( void);
MSG_DESC *msg_Identify( const byte *);
int msg_Init( void);
int msg_Send( byte *msg);

//  Encode functions
void gps_Reset(void);
void gps_AlmanacSetup(char *);
void gps_ModeSetup(void);
void gps_PosSetup(POS *pos);
void gps_TimeSetup(struct tm *tm);
void gps_UnitsSetup(UNITS *units);

void gps_AlmanacStart(void);
void gps_CourseStart(void);
void gps_ReceiverStart(void);
void gps_PosStart(void);
void gps_SatsUsedStart(void);
void gps_SatStatStart(void);
void gps_SatUsageStart(void);
void gps_TimeStart(void);

void gps_AlmanacStop(void);
void gps_CourseStop(void);
void gps_PosStop(void);
void gps_ReceiverStop(void);
void gps_SatsUsedStop(void);
void gps_SatStatStop(void);
void gps_SatUsageStop(void);
void gps_TimeStop(void);

int gps_ReceiverGet(RCV *buffer);
int gps_PosGet(POS *buffer);
int gps_NativePosGet(NATIVE_POS *buffer);
int gps_CourseGet(COURSE *buffer);
int gps_AlmanacGet(char *buffer);
int gps_EphemerisGet(char *buffer);
int gps_SatsUsedGet(int *buffer);
int gps_SatStatGet(SATSTAT *buffer);
int gps_SatUsageGet(SATUSAGE *buffer);
int gps_TimeGet( struct tm *buffer);

int gps_Initialize(void);
void gps_Terminate(void);

#ifdef __cplusplus
}
#endif

#endif

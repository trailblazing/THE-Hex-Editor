#ifndef _ASD_H_
#define _ASD_H_


// Spectrum data type (variable data_type at byte offset 186):

#define RAW_TYPE        (byte)0
#define REF_TYPE        (byte)1
#define RAD_TYPE        (byte)2
#define NOUNITS_TYPE    (byte)3
#define IRRAD_TYPE      (byte)4
#define QI_TYPE         (byte)5
#define TRANS_TYPE      (byte)6
#define UNKNOWN_TYPE    (byte)7
#define ABS_TYPE        (byte)8

// Spectrum data format (variable data_format at byte offset 199):

#define FLOAT_FORMAT    (byte)0
#define INTEGER_FORMAT  (byte)1
#define DOUBLE_FORMAT   (byte)2
#define UNKNOWN_FORMAT  (byte)3

// Instrument type that created spectrum (variable instrument at byte offset 431):

#define UNKNOWN_INSTRUMENT          (byte)0
#define PSII_INSTRUMENT             (byte)1
#define LSVNIR_INSTRUMENT           (byte)2
#define FSVNIR_INSTRUMENT           (byte)3
#define FSFR_INSTRUMENT             (byte)4
#define FSNIR_INSTRUMENT            (byte)5
#define CHEM_INSTRUMENT             (byte)6
#define FSFR_UNATTENDED_INSTRUMENT  (byte)7

// Appendix III: Details of data structures referenced in Appendix I


struct _tm  // tm defined by time.h of c++ lib, have another two field
{
    int tm_sec;       // seconds [0,61]=00(160)
    int tm_min;       // minutes [0,59]=38(162)
    int tm_hour;      // hour [0,23]=13(164)
    int tm_mday;      // day of month [1,31]=27(166)
    int tm_mon;       // month of year [0,11]=03(168)
    int tm_year;      // years since 1900=2013(170)
    int tm_wday;      // day of week [0,6] (Sunday = 0)=6(172)
    int tm_yday;      // day of year [0,365]=116(174)
    int tm_isdst;     // daylight savings flag=1(176)

    friend inline std::ostream & operator << (std::ostream & out, const _tm & tm)
    {
        out << tm.tm_year + 1900 << ":";
        out << std::setfill('0') << std::setw(2)<< tm.tm_mon << ":";
        out << std::setfill('0') << std::setw(2)<< tm.tm_mday << ":";
        out << std::setfill('0') << std::setw(2)<< tm.tm_hour << ":";
        out << std::setfill('0') << std::setw(2)<< tm.tm_min << ":";
        out << std::setfill('0') << std::setw(2)<< tm.tm_sec << endl;
        out << "day of week =\t" << tm.tm_wday << endl;
        out << "day of year =\t" << tm.tm_yday << endl;
        out << "daylight saving flag =\t" << hex << tm.tm_isdst << endl;

        return out;
    }
};






//typedef long time_t;

// APP_DATA

// This is a 128 byte field that is used for storing results produced by various real-time processing routines.

struct _GPS_DATA
{
    double  true_heading;
    double  speed;
    double  latitude, longitude;
    double  altitude;
    struct
    {
        unsigned havecomm : 1;
        unsigned terrain : 2;
        unsigned datum : 6;
        unsigned dist_sp_units : 2;
        unsigned alt_units : 2;
        unsigned mag_var : 2;
        unsigned nav : 1;
    } flags; // these are bit fields totaling to 2 bytes

    char hardware_mode;


    time_t timestamp;


    struct
    {
        unsigned corrected  : 1;
        unsigned filler     : 15;

    } flags2; // these are bit fields totaling to 2 bytes unsigned char satellites[5];

    char filler[2];
};

typedef struct{
//?
}UNATTENDED_DATA;

//typedef union{
//// unsigned char
//}byte;
//
//typedef bool boolean;   // unsigned char

typedef unsigned char byte;
typedef unsigned char boolean;

#endif

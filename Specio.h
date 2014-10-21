/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Header	    :specio.h
Created By  :Steve Simms
Description :Header for ASD spectrum files Modified:
	21 Jan 93
		Modified all write_ routines so they accept overwrite mode.
	5 Feb 92	Simms
		Upgraded to v3.2 supporting axis mins and maxs, ip_numofbits
		and xmode.
	NOTE! -- Modification 3 --
		This is the modification num of the specio package including:
		specio.* fileio.* and head.*. If ANY of these change, this
		modification number should be updated. Eventually these will
		all be in a	library with the "version" number idea being follow
		through!

    10 Feb 93   Mike        -- Modification 4 --
        Changed scaling of integer-format reflectance files to keep
        4095 as 100% instead of 150%.

    17 Feb 93   Mike        -- Modification 5 --
        Removed code from head_Update so that when x and y scale limits
        are not known (ie, old PSII files), they are left all equal to 0.0.
        Thus, when a scale's difference (max-min) is 0.0, the limits weren't
        saved to the file.

    15 Apr 93   Mike
        Created file version 3.4 removing ratio_file[] field and replacing
        with expanded GPS structure. File version 3.3 contained a new 'flags'
        structure. Changed sample_count to an integer instead of a byte.

    20 Sep 93   Mike    Header for version 1.06
        Removed IP.H and DETECTOR_SIZE dependencies

	Dec 1, 1993 Steve
		Created file version 4.0 for fieldspec instruments
		Changed so now the size of the spectrum array is
		acquired from the header.
		Added the field - byte instrument - and the define *_INSTRUMENT.
		Removed GPS_APP field.

	May 24 94 Steve v4.01
		Added absorbance and transmittance modes

    Jan 12 98 trg v4.01
        Added META_DATA and UNATTENDED_DATA for unattended operations.

    June 10 98 trg
        Add support for high-precision time and fix version number in header.

    Aug  30 02 beb
        Add Absolute Reflectance type and contant

Copyright (C) 1990. Analytical Spectral Devices (ASD).
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if !defined(_TIME_T_DEFINED)	// MSC define
#include <time.h>
#endif

#if !defined(_COMMON_H)          // common.h define
//#include <common.h>
#endif

#if !defined(_UNATTEN_H)
//#include "unatten.h"
#endif


#define NO_APP      0           // No application information stored
#define BANDS_APP   1           /* bands application */
#define CI_APP      2           /* colorimetry application */
#define CALIB_APP   3           /* calibration application */
#define CHEM_APP    4
#define UNATT_APP   5
#define ABSOLUTE_REF_APP  6     /* Absolute Reflectance application type */

#pragma pack(1)                 // MSC needs, pack it wo/word allignment


typedef unsigned short int ushort;

typedef unsigned long int ulong;

typedef unsigned int uint;

typedef struct color_data {
    float nx2, nx10;
    float ny2, ny10;
    float u_prime_2, u_prime_10;
    float v_prime_2, v_prime_10;
    char ill_file[12];
} COLOR_DATA;

typedef struct band_data {
    float bands[8];
} BAND_DATA;

typedef struct cal_data {
    char nothing;
} CAL_DATA;

    /** a union takes on the size of its largest element.  This filler
        makes sure that we have room for the other structure to grow. **/
typedef struct {
	char data[128];
} FILLER;

typedef struct InStep_header {
	int classNum;
	int regressNum;
	int resultNum;
	int macroSize;
} INSTEP_HDR;

typedef struct {
    char arName[31];        // Name of the Absolute Reflectence
    char arFileName[31];    // File Name to Load
} AR_DATA;

    /* and this is the union to create one generic name for the application-
       specific data */
typedef union {
    COLOR_DATA      ci_data;
    BAND_DATA       band_data;
    CAL_DATA        cal_data;
    UNATTENDED_DATA unattended_data ;
    INSTEP_HDR chem_data;
    AR_DATA         ar_data;
    FILLER          filler;
} APP_DATA;

#define COMMENT_SIZE 157		// length of the comment field

typedef struct sp_hdr {         // ASD spectrum header description (3.4)
	char	co[3];				// Three character company name (1.0)
	char    comments[ COMMENT_SIZE];	// comment field (1.0)
    struct	tm when;    		// time when spectrum was saved (1.0)
    byte    program_version;    // version of the program which created
								// this file. major version in upper nibble
								// minor version in lower (2.0)
    byte    file_version;		// spectrum file format version (1.0)
    byte    itime;              // Not used after v2.00 (1.0)
    byte    dc_corr;			// 1 if DC subtracted, 0 if not (1.0)
    time_t  dc_time;            // Time of when last dc was taken (1.0)
	byte    data_type;          // see *_TYPE below (1.0)
    time_t  ref_time;           // Time of when last wr was take (1.0)
	float   ch1_wavel;  		// calibrated starting wavelength in nm (1.0)
	float   wavel_step; 		// calibrated wavelength step in nm (1.0)
    byte    data_format;        // format of spectrum. See *_FORMAT (1.0)
    byte    old_dc_count;		// Num of DC measurements in the avg (2.0-3.3)
    byte    old_ref_count;		// Num of WR in the average (2.0 - 3.3)
    byte    old_sample_count;	// Num of spec samples in the avg (2.0-3.3)
    byte    application;		// Which application created APP_DATA
    ushort	channels;			// Num of channels in the detector (2.0)
    APP_DATA  app_data;         // Application-specific data
//	byte	gps_spare[496]		// Spare from GPS app in PSII. Remove in 4.00
    GPS_DATA  gps_data;         // GPS position, course, etc.
	ulong	it;					// The actual integration time in ms (3.0)
	int		fo;					// The for attachment's view in degrees (3.0)
	int		dcc;				// The dark current correction value (3.0)
//	long    id;					// unique id number (3.1) (remove in 4.00)
	uint	calibration;		// calibration series (4.00)
	uint	instrument_num;		// instrument number (4.00)
	float	ymin;				// setting of the y axis' min value (3.2)
	float   ymax;				// setting of the y axis' max value (3.2)
	float  	xmin;				// setting of the x axis' min value (3.2)
	float	xmax;				// setting of the x axis' max value (3.2)
	uint	ip_numbits;			// instrument's dynamic range  (3.2)
	byte	xmode;				// x axis mode. See *_XMODE (3.2)
    byte    flags[4];           // Flags (0 = AVGFIX'ed) (3.3)
    unsigned     dc_count;		// Num of DC measurements in the avg (2.0/3.4)
    unsigned     ref_count;		// Num of WR in the average (2.0/3.4)
    unsigned     sample_count;	// Num of spec samples in the avg (2.0/3.4)
	byte	instrument;			// Instrument type. See defs below (4.00)
	ulong	bulb;				// The id number of the cal bulb (4.00)
	uint	swir1_gain;			// gain setting for swir 1 (4.00)
	uint	swir2_gain;			// gain setting for swir 2 (4.00)
	uint	swir1_offset;   	// offset setting for swir 1 (4.00)
	uint	swir2_offset;   	// offset setting for swir 2 (4.00)
	float	splice1_wavelength;	// wavelength of VNIR and SWIR1 splice (4.00)
	float	splice2_wavelength; // wavelength of SWIR1 and SWIR2 splice (4.00)
    char    when_in_ms[12];     // fill to 484 bytes
    byte    spare[20];          // fill to 484 bytes
} HDR;
								// The instrument type that created the spec
#define UNKNOWN_INSTRUMENT          (byte)0
#define PSII_INSTRUMENT             (byte)1
#define LSVNIR_INSTRUMENT           (byte)2
#define FSVNIR_INSTRUMENT           (byte)3
#define FSFR_INSTRUMENT             (byte)4
#define FSNIR_INSTRUMENT            (byte)5
#define CHEM_INSTRUMENT             (byte)6
#define FSFR_UNATTENDED_INSTRUMENT  (byte)7 // for compatibility with Chem

                                // available format of the save spectrum
#define FLOAT_FORMAT    (byte)0
#define INTEGER_FORMAT  (byte)1
#define DOUBLE_FORMAT	(byte)2
#define UNKNOWN_FORMAT	(byte)3
								// The types of spectrum that can be calc'ed
#define RAW_TYPE        (byte)0
#define REF_TYPE        (byte)1
#define RAD_TYPE        (byte)2
#define NOUNITS_TYPE    (byte)3
#define IRRAD_TYPE      (byte)4
#define QI_TYPE			(byte)5
#define TRANS_TYPE		(byte)6
#define UNKNOWN_TYPE    (byte)7
#define ABS_TYPE		(byte)8

#define WAVELENGTH_XMODE	(byte)0
#define CHANNEL_XMODE		(byte)1
#define UNKNOWN_XMODE		(byte)2

extern char *type_names[];

#define FILE_VERSION                0x43    // Spectrum file version
                                            // and return to default packing alignment
#define FILE_VERSION_HP_TIME        0x42    // First version with high-precision time
#define FILE_VERSION_VERSION_FIX    0x43    // Fixes version in header.
#pragma pack()
								// ptr to error mgs
extern char specio_errstr[];
								// Global routines
//boolean spio_CheckFile( char *filename);
//boolean spio_WriteSpec( char *filename, HDR *hdr, double *spec , byte type
//   , boolean o_mode , boolean a_mode );
//
//boolean spio_ReadIrr( char *filename, HDR *hdr, double **spec);
//boolean spio_ReadRad( char *filename, HDR *hdr, double **spec);
//boolean spio_ReadRaw( char *filename, HDR *hdr, double **spec);
//boolean spio_ReadRef( char *filename, HDR *hdr, double **spec);
//boolean spio_ReadQi( char *filename, HDR *hdr, double **spec);
//boolean spio_ReadAbs( char *filename, HDR *hdr, double **spec);
//boolean spio_ReadTrans( char *filename, HDR *hdr, double **spec);
//boolean spio_ReadSpec( char *filename, HDR *hdr, double **spec, byte *type);

// replace boolean to bool

bool spio_CheckFile( char *filename);
bool spio_WriteSpec( char *filename, HDR *hdr, double *spec , byte type
   , bool o_mode , bool a_mode );

bool spio_ReadIrr( char *filename, HDR *hdr, double **spec);
bool spio_ReadRad( char *filename, HDR *hdr, double **spec);
bool spio_ReadRaw( char *filename, HDR *hdr, double **spec);
bool spio_ReadRef( char *filename, HDR *hdr, double **spec);
bool spio_ReadQi( char *filename, HDR *hdr, double **spec);
bool spio_ReadAbs( char *filename, HDR *hdr, double **spec);
bool spio_ReadTrans( char *filename, HDR *hdr, double **spec);
bool spio_ReadSpec( char *filename, HDR *hdr, double **spec, byte *type);


byte spio_GetType( char *filename);
HDR *spio_InitHeader( HDR *hdr);

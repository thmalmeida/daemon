#include <cstdint>

/* By thmalmeida on 20240820
*  Ref.: https://gpsd.gitlab.io/gpsd/NMEA.html
*/
class GNSS {
public:

	//(GN)GGA - Global Positioning System Fix Data - Time, Position and fix related data for a GPS receiver.
	// 1 - utc of this position report
	int utc_hour;
	int utc_min;
	int utc_sec;
	uint32_t utc_time;		// Field 1 - UTC of this position report, hh is hours, mm is minutes, ss.ss is seconds;

	int lat_deg;			// Field 2 - Latitude, dd is degrees, mm.mm is minutes
	double lat_min;
	double lat_dec;
	char lat_index = '_';	// Field 3 - N or S (North or South)

	int lon_deg;			// Field 4 - Longitude, dd is degrees, mm.mm is minutes 
	double lon_min;	
	double lon_dec;
	char lon_index = '_';	// Field 5 - E or W (East or West)

	int quality;			/* Field 6 - GPS Quality Indicator (non null)
								0 - fix not available,
								1 - GPS fix,
								2 - Differential GPS fix (values above 2 are 2.3 features)
								3 = PPS fix
								4 = Real Time Kinematic
								5 = Float RTK
								6 = estimated (dead reckoning)
								7 = Manual input mode
								8 = Simulation mode
							*/
	int n_satts;			// Field 7 - number of sattelites in use
	double hdop = 0.0;		// Field 8 - Horizontal Dilution of Precision (DOP) (meters)
	int altitude = 0; 		// Field 8 - Antenna Altitude above/below mean-sea-level (geoid) (in meters)
	char units_pos = 0;		// Field 10- Units of antenna altitude, meters

	// GPGSA or __GSA - GPS DOP and active satellites
	struct {
		char sel_mode = '_';			// Field 1 - Selection mode: M=Manual, forced to operate in 2D or 3D, A=Automatic, 2D/3D
		uint8_t mode = 0;				// Field 2 - Mode (1 = no fix, 2 = 2D fix, 3 = 3D fix)
		int satt_id_group[12] = {0};	// Field 3 to 14 - ID of 1st to 12th satellite used for fix
		double pdop;					// Field 15
		double hdop;					// Field 16
		double vdop;					// Field 17
		uint8_t sys_id;					// Field 18 - System ID (NMEA 4.11), see above
											// 1 = GPS L1C/A, L2CL, L2CM
											// 2 = GLONASS L1 OF, L2 OF
											// 3 = Galileo E1C, E1B, E5 bl, E5 bQ
											// 4 = BeiDou B1I D1, B1I D2, B2I D1, B2I D12
	} gnss_sys[4];

	// GNRMC or __RMC - Recommended Minimum Navigation Information

	void str_parse(void) {

	}

};
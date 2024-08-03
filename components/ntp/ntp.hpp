#ifndef NTP_HPP__
#define NTP_HPP__

/* Network Time Protocol (NTP)
 * Ref.: https://datatracker.ietf.org/doc/html/rfc958
 *
 */


/* NTPv4 RFC 5905 replaces RFC 1305 (NTPv3)
* Ref.: https://www.rfc-editor.org/info/rfc5905
* 
* 
*/
#include <cstdint>

#define NTP_SERVER_0	"thmalmeida.us.to"
#define NTP_SERVER_1	"pool.ntp.org"
#define NTP_SERVER_2	"a.st1.ntp.br"
#define NTP_SERVER_3	"gps.ntp.br"

// Total of 48 bytes
typedef struct {

	uint8_t li_status;				// Leap indicator is a two-bit code. Status is a six-bit code

	uint8_t ref_clock_type;			// Reference clock type. Stratum level
	int16_t precision;				// Precision

	uint32_t est_error;				// Estimate error. Fixed point between bit 15 and 16
	int32_t est_drift_rate;			// Estimated Drift Rate
	uint32_t ref_clock_id;			// Reference Clock Identifier

	// MSB 32-bit integer and LSBits 32-bit fractional part
	uint64_t ref_timestamp;			// Reference Timestamp

	uint64_t orig_timestamp;		// Originate Timestamp

	uint64_t receive_timestamp;		// Receive Timestamp

	uint64_t transmit_timestamp;	// Transmit timestamp

} ntp_packet;


class NTP_Time{
public:
	NTP_Time() {
		init();
	}

	~NTP_Time() {

	}

	void init() {
	}

	void sync() {

	}

private:

};

#endif /* SNTP_TIME_HPP__ */

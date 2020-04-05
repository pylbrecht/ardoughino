#define RF_DATA_PIN 2
#define PROTO_BITS 24

/* ON codes:
 * 38704
 * 970816
 * 689264
 * 988080
 */
const uint32_t ON = 38704;

/* OFF codes:
 * 865568
 * 389648
 * 608656
 * 182432
 */
const uint32_t OFF = 865568;

typedef struct {
	uint32_t dt_high;
	uint32_t dt_low;
} Pulse;

typedef struct {
	Pulse preamble;
	Pulse one;
	Pulse zero;
} Config;


void transmit(Pulse);
void transmit(uint32_t);
void turn_on();
void turn_off();


void setup() {
	pinMode(RF_DATA_PIN, OUTPUT);
	digitalWrite(RF_DATA_PIN, LOW);
}

void loop() {
	delay(2000);
	turn_on();
	delay(2000);
	turn_off();
}

void turn_on() {
	Config cfg = {
		{300, 2300},
		{1050, 450},
		{300, 1200}
	};
	transmit(ON, PROTO_BITS, cfg);
	cfg = {
		{2950, 7150},
		{930, 580},
		{430, 1080}
	};
	transmit(ON, PROTO_BITS, cfg);
};

void turn_off() {
	Config cfg = {
		{300, 2300},
		{1050, 450},
		{300, 1200}
	};
	transmit(OFF, PROTO_BITS, cfg);
	cfg = {
		{2950, 7150},
		{930, 580},
		{430, 1080}
	};
	transmit(OFF, PROTO_BITS, cfg);
};

void transmit(uint32_t code, uint8_t num_bits, Config cfg) {
	for (int repeat = 0; repeat < 4; repeat++) {
		transmit(cfg.preamble);
		for (int bit = num_bits - 1; bit > -1; bit--) {
			if (bitRead(code, bit)) {
				transmit(cfg.one);
			}
			else {
				transmit(cfg.zero);
			}
		}
	}
}

void transmit(Pulse pulse) {
	digitalWrite(RF_DATA_PIN, HIGH);
	delayMicroseconds(pulse.dt_high);
	digitalWrite(RF_DATA_PIN, LOW);
	delayMicroseconds(pulse.dt_low);
}

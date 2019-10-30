#ifndef INCLUDE_STRUCTS_H
#define INCLUDE_STRUCTS_H

typedef struct agent_t {
	bool arrow;
	bool grito;
	bool coord;
	bool limit;
} Sensor;

typedef struct room_t {
	bool wumpus;
	bool fedor;
	bool whell;
	bool brisa;
	bool gold;
	bool limit;
} Room;

typedef struct sensor_t {
	bool fedor;
	bool brisa;
	bool gold;
	bool limit;
} Sensor;

//#pragma message __FILE__
#endif // INCLUDE_STRUCTS_H
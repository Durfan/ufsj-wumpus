#ifndef INCLUDE_STRUCTS_H
#define INCLUDE_STRUCTS_H

typedef struct room_t {
	bool wumpus;
	bool fedor;
	bool whell;
	bool brisa;
	bool gold;
	bool reluz;
	bool limit;
} Room;

typedef struct sensor_t {
	bool fedor;
	bool brisa;
	bool reluz;
	bool limit;
	bool grito;
} Sensor;

//#pragma message __FILE__
#endif // INCLUDE_STRUCTS_H
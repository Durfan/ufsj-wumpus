#ifndef INCLUDE_STRUCTS_H
#define INCLUDE_STRUCTS_H

typedef struct agent_t {
	int score;
	bool arrow;
	bool grito;
	bool coord;
	bool limit;
} Agent;

typedef struct room_t {
	int id;
	int grau;
	bool wumpus;
	bool fedor;
	bool whell;
	bool brisa;
	bool gold;
	bool limit;
} Room;

typedef struct bknow_t {
	int id;
	int grau;
	bool wumpus;
	bool fedor;
	bool whell;
	bool brisa;
	bool gold;
	bool limit;
} Bknow;

typedef struct sensor_t {
	bool fedor;
	bool brisa;
	bool gold;
	bool limit;
} Sensor;

//#pragma message __FILE__
#endif // INCLUDE_STRUCTS_H
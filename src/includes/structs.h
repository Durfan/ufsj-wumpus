#ifndef INCLUDE_STRUCTS_H
#define INCLUDE_STRUCTS_H

typedef struct agent_t {
	int coord;
	int score;
	bool arrow;
	bool grito;
	bool limit;
} Agent;

typedef struct room_t {
	int id;
	int grau;
	bool ghost;
	bool smell;
	bool traps;
	bool wind;
	bool gold;
	bool lmit;
} Room;

typedef struct bknow_t {
	int id;
	int grau;
	int ghost;
	int traps;
	bool visited;
	bool smell;
	bool wind;
	bool gold;
	bool lmit;
} Bknow;

typedef struct sensor_t {
	bool smell;
	bool wind;
	bool gold;
	bool lmit;
} Sensor;

//#pragma message __FILE__
#endif // INCLUDE_STRUCTS_H
#ifndef INCLUDE_STRUCTS_H
#define INCLUDE_STRUCTS_H

typedef enum { talvez=-1, nope, certeza, noinf } TriBol;
typedef enum { explore = -1, killer, gotodie} Intern;

typedef struct agent_t {
	int coord;
	int score;
	int ghost;
	bool lives;
	bool arrow;
	bool grito;
	bool limit;
	int killerTarget;

	Intern state;

} Agent;

typedef struct quad_t {
	bool ghost;
	bool smell;
	bool traps;
	bool  wind;
	bool  gold;
} Quad;

typedef struct know_t {
	bool visit;
	int paths;
	TriBol ghost;
	bool smell;
	TriBol traps;
	bool wind;
	bool gold;
} Know;

typedef struct sensor_t {
	bool smell;
	bool wind;
	bool gold;
} Sensor;

//#pragma message __FILE__
#endif // INCLUDE_STRUCTS_H

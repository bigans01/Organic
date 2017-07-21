// adding guards
#ifndef MOVEGOAL_H
#define MOVEGOAL_H

class BasicEntity; // forward declarations from BasicEntity.h file 

class Movegoal {
	float origin_x, origin_y, origin_z; // original coords from when this goal was instantiated
	float dest_x, dest_y, dest_z;		// destination coords
										//float curr_x, curr_y, curr_z;		// current coords 

public:

	virtual void calculate(BasicEntity* entity) = 0;	    // 0 indicates pure virtual function
};

#endif
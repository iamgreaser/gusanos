#ifndef WORM_H
#define WORM_H

//#include "base_object.h"
#include "base_worm.h"
//#include "sprite.h"

class BaseAnimator;
class BasePlayer;
class NinjaRope;

class Worm : public BaseWorm
{	
public:
	Worm();
	~Worm();

	void think();

private:
};

#endif  // _WORM_H_

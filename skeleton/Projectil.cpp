#include "Projectil.h"
#include <math.h> 

Projectil::Projectil(Vector3 pos, Vector3 real_velocity,double real_mass)
	:Particle(pos, {0,0,0}, { 0,-9.8,0 }, EULER_SEMI_IMPLICITO)
{
	//_vel = std::clamp(real_velocity, 5, 25);
	//_mass = 0;
}

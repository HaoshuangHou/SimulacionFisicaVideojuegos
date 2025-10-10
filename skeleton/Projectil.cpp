#include "Projectil.h"
#include <math.h> 
#include <algorithm> 

const Projectil::ProjectilData Projectil::proyectilesDB[] = {
   //MasaReal VelReal 
    {5.0,     250.0},       //Canon
    {100.0,    250.0},      //Tanque
    {0.2,    330.0},        //Pistola
    {0.001,   300000000.0}  //Laser
};


Projectil::Projectil(Vector3 pos, Vector3 sim_velocity,ProjectilType t)
	:Particle(pos, sim_velocity, { 0,0,0 }, EULER_SEMI_IMPLICITO)
{
    configurarDesdeTipo(t, sim_velocity);
    
}

void Projectil::configurarDesdeTipo(ProjectilType T, Vector3 sim_velocity)
{
    const ProjectilData& datosReales = proyectilesDB[T];

    // Masa simulada
    double sim_speed = sim_velocity.magnitude();
    if (sim_speed < 0.1) sim_speed = 0.1;

    _mass = (datosReales.real_mass * datosReales.real_speed * datosReales.real_speed) / (sim_speed * sim_speed);

    //gravedad
    float gravedad_y = 0.0f;  // Por defecto CERO gravedad

    if (datosReales.real_speed > 1000.0)
        gravedad_y = 0.0f;  
    else if (datosReales.real_speed > 500.0)
        gravedad_y = -0.1f;   
    else if (datosReales.real_speed > 200.0)
        gravedad_y = -0.5f;   
    else
        gravedad_y = -2.0f;

    _ac = { 0, gravedad_y, 0 };
}

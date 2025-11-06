#include "Projectil.h"
#include <math.h> 
#include <algorithm> 
#include <iostream> 

const Projectil::ProjectilData Projectil::proyectilesDB[] = {
   //MasaReal VelReal 
    {5.0,     250.0},       //Canon
    {100.0,    250.0},      //Tanque
    {0.2,    330.0},        //Pistola
    {0.001,   300000000.0}, //Laser
    {0.2,   30}  //game
};


Projectil::Projectil(Vector3 pos, Vector3 sim_velocity, ProjectilType t, Vector4 const& color)
    :Particle(pos, sim_velocity, { 0,0,0 }, color, EULER_SEMI_IMPLICITO)
{
    configurarDesdeTipo(t, sim_velocity);
}

void Projectil::update(double t)
{
    Particle::update(t);
}

void Projectil::configurarDesdeTipo(ProjectilType T, Vector3 sim_velocity)
{
    const ProjectilData& datosReales = proyectilesDB[T];

    // Masa simulada
    double sim_speed = sim_velocity.magnitude();
    if (sim_speed < 0.1) sim_speed = 0.1;

    _mass = (datosReales.real_mass * datosReales.real_speed * datosReales.real_speed) / (sim_speed * sim_speed);
}

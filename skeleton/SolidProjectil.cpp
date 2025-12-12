//#include "SolidProjectil.h"
//#include <math.h> 
//#include <algorithm> 
//#include <iostream> 
//
//const SolidProjectil::ProjectilData Projectil::proyectilesDB[] = {
//   //MasaReal VelReal 
//    {5.0,     250.0},       //Canon
//    {100.0,    250.0},      //Tanque
//    {0.2,    330.0},        //Pistola
//    {0.001,   300000000.0}, //Laser
//    {0.2,   30}  //game
//};
//
//
//SolidProjectil::SolidProjectil(Vector3 pos, Vector3 sim_velocity, Vector4 const& color)
//    :SolidEntity(pos, sim_velocity, { 0,0,0 }, color), _real_speed(30),_real_mass(0.2)
//{
//    //physx::PxPhysics* physics, physx::PxScene* scene,
//    bool dynamic, const physx::PxVec3& pos,
//        const physx::PxGeometry& geometry, double density,
//        physx::PxMaterial* material, const Vector4& color
//
//    configurarDesdeTipo(t, sim_velocity);
//
//    double sim_speed = sim_velocity.magnitude();
//    if (sim_speed < 0.1) sim_speed = 0.1;
//
//    _mass = (_real_mass * _real_speed * _real_speed) / (sim_speed * sim_speed);
//}
//
//void SolidProjectil::update(double t)
//{
//    SolidEntity::update(t);
//}

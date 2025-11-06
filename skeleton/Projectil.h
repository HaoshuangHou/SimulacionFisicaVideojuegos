#pragma once
#include "Particle.h"
enum ProjectilType {
    CANON, TANQUE, PISTOLA, LASER, GAME
};
class Projectil: public Particle
{
public:
    Projectil(Vector3 pos, Vector3 sim_velocity, ProjectilType T, Vector4 const& color = Vector4(1, 1, 1, 1));
    virtual void update(double t) override;
private:
    struct ProjectilData {
        double real_mass;
        double real_speed; 
    };

    static const ProjectilData proyectilesDB[];

    void configurarDesdeTipo(ProjectilType T, Vector3 sim_velocity);
};


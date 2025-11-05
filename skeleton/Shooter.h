#pragma once
#include "Entity.h"
#include "ForceGenerator.h"

class Shooter : public Entity
{
private:
    float _currentAngle;
    Vector3 _basePosition;
public:
    Shooter(const Vector3& pos, const Vector4& color);
    virtual ~Shooter() = default;

    void update(double t) override;

    void setAngle(float angle);
    void rotate(float deltaAngle);
    float getAngle() const { return _currentAngle; }

    // Para disparar
    Vector3 getPosition() const;
    Vector3 getShootDirection() const;

    // Posici¨®n base (para referencia)
    Vector3 getBasePosition() const { return _basePosition; }
};


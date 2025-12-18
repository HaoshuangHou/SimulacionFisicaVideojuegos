#pragma once
#include "Entity.h"
#include <vector>
#include <memory>

class Fish : public Entity {
public:
    enum Part{ BODY, TAIL, EYE, MOUTH, NUM_PARTS };

    Fish(const Vector3& pos, const Vector4& color, float scale = 1);
    ~Fish();

    // rotacion
    void setAngle(float angle);
    void rotate(float deltaAngle);
    float getAngle() const { return _currentAngle; }

    //para disparar
    Vector3 getPosition() const;
    void setPosition(const Vector3& pos) override;
    Vector3 getShootDirection() const;

    virtual void create_renderItem()override;
    virtual void deregisterRenderItem()override;

private:
    void updateParts();
    std::vector<std::unique_ptr<Entity>> _parts;
    float _currentAngle = 0.0f;

    Vector3 bodyOffset = { 1.5f, 0.0f, 0.0f };
    Vector3 tailOffset = { 0.0f, 0.0f, 0.0f };
    Vector3 eyeOffset = { 1.8f, 0.3f, 0.01f };
    Vector3 mouthOffset = { 2.8f, 0.0f, 0.1f };
};

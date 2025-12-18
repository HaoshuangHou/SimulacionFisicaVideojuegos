#pragma once
#include "core.hpp"
#include "Particle.h"
#include "SolidEntity.h"
#include "RenderUtils.hpp"

template <typename T>
class ForceGenerator
{
private:
    bool _isRegistered;

protected:
    Vector3 _center;
    double _radius;
	RenderItem* _renderItem ;

    void createRenderItem(physx::PxShape* shape, const Vector3& position, const Vector4& color) {
        if (_renderItem != nullptr) {
            if (_isRegistered) {
                DeregisterRenderItem(_renderItem);
                _isRegistered = false;
            }
            
            _renderItem;
        }
        _renderItem = new RenderItem(shape, new physx::PxTransform(position), color);
        RegisterRenderItem(_renderItem);
        _isRegistered = true;
    }

public:
	ForceGenerator() :_renderItem(nullptr), _isRegistered(false), _center(), _radius() {};
	virtual ~ForceGenerator() { 
        if (_renderItem != nullptr) {
            if (_isRegistered) {
                DeregisterRenderItem(_renderItem);
                _isRegistered = false;
            }
            delete _renderItem;
            _renderItem = nullptr;
        }
    
    };
	virtual void updateForce(T* particle, double dt) = 0;

    Vector3 getCenter() { return _center; };
    double getRadius() { return _radius; };

    void setVisible(bool visible) {
        if (_renderItem != nullptr) {
            if (visible && !_isRegistered) {
                RegisterRenderItem(_renderItem);
                _isRegistered = true;
            }
            else if (!visible && _isRegistered) {
                DeregisterRenderItem(_renderItem);
                _isRegistered = false;
            }
        }
    }
};


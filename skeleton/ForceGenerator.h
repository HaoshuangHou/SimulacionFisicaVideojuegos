#pragma once
#include "core.hpp"
#include "Particle.h"
#include "RenderUtils.hpp"

class ForceGenerator
{
private:
    bool _isRegistered;
protected:
	RenderItem* _renderItem ;

    void createRenderItem(physx::PxShape* shape, const Vector3& position, const Vector4& color) {
        if (_renderItem != nullptr) {
            if (_isRegistered) {
                DeregisterRenderItem(_renderItem);
                _isRegistered = false;
            }
            delete _renderItem;
        }

        _renderItem = new RenderItem(shape, new physx::PxTransform(position), color);
        RegisterRenderItem(_renderItem);
        _isRegistered = true;
    }

public:
	ForceGenerator() :_renderItem(nullptr), _isRegistered(false) {};
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
	virtual void updateForce(Particle* particle, double dt) = 0;

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


#pragma once
#include"libs.h"

class Collider {
protected:
	glm::vec3* colliderCentre = NULL; ///<Centre point for the collider @note Usually the same as the gameobject it is a collider for.
public:
	virtual float X() = 0;
	virtual float Y() = 0;
	virtual float Z() = 0;

	virtual bool collidesWith(Collider* other) = 0; ///<pure virtual function. every collider must implement collidesWith, which calculates whether this collider collides with other.

	virtual void Draw() = 0; ///<Draw method for debug mode. Must override in inherited classes.
};

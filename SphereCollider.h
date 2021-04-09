#pragma once
#include "Collider.h"
#include"libs.h"

using namespace std;

class SphereCollider : public Collider {
private:
public:
	float sphereRadius;

	glm::vec3 distance;

	SphereCollider(glm::vec3* position, float radii);
	float X();
	float Y();
	float Z();
	~SphereCollider(); ///<Default destructor.

	bool collidesWith(Collider* other);  ///< Implementation of fuction deciding if this collider collides with the other one.
	void Draw();
};


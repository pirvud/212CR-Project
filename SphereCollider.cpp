#include "SphereCollider.h"
#include "Game.h"

SphereCollider::SphereCollider(glm::vec3* position, float radii) {
	this->colliderCentre = position;
	this->sphereRadius = radii;
}

float SphereCollider::X() {
	return this->colliderCentre->x;
}

float SphereCollider::Y() {
	return this->colliderCentre->y;
}

float SphereCollider::Z() {
	return this->colliderCentre->z;
}

SphereCollider::~SphereCollider() {
}

bool SphereCollider::collidesWith(Collider* other) {
	if (other == NULL) {
		return false;
	}
	float distanceX = fabs(this->X() - other->X()); //fabs is the equivalent of the absolute "modifier"
	float distanceZ = fabs(this->Z() - other->Z());

	return distanceX + distanceZ <= (sphereRadius * 2);
}

//Draw sphere with vertices so that we can show it in debug mode.
void SphereCollider::Draw()
{

}

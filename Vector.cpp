
#include "stdafx.h"
#include "Vector.h"


float Vector2::Distance(const Vector2 &startVec, const Vector2 &endVec) {
	return sqrtf(Vector2::DistanceSquared(startVec, endVec));

}
float Vector2::DistanceSquared(const Vector2 &startVec, const Vector2 &endVec) {
	return (endVec.x - startVec.x) * (endVec.x - startVec.x) + (endVec.y - startVec.y) * (endVec.y - startVec.y);
}

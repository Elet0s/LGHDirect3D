#include "PreCompile.h"
#include "GameEngineTransform.h"

bool GameEngineTransform::SphereToSphere(const GameEngineTransform& _sphereA, const GameEngineTransform& _sphereB)
{	
	return _sphereA.collisionDataObject_.sphere_.Intersects(_sphereB.collisionDataObject_.sphere_);
}

bool GameEngineTransform::AABBToAABB(const GameEngineTransform& _boxA, const GameEngineTransform& _boxB)
{
	return _boxA.collisionDataObject_.aabb_.Intersects(_boxB.collisionDataObject_.aabb_);
}

bool GameEngineTransform::OBBToOBB(const GameEngineTransform& _boxA, const GameEngineTransform& _boxB)
{
	return _boxA.collisionDataObject_.obb_.Intersects(_boxB.collisionDataObject_.obb_);
}

bool GameEngineTransform::Sphere2DToSphere2D(const GameEngineTransform& _sphereA, const GameEngineTransform& _sphereB)
{
	DirectX::BoundingSphere sphereA = _sphereA.collisionDataObject_.sphere_;
	DirectX::BoundingSphere sphereB = _sphereB.collisionDataObject_.sphere_;

	sphereA.Center.z = 0.f;
	sphereB.Center.z = 0.f;

	return sphereA.Intersects(sphereB);
}

bool GameEngineTransform::AABB2DToAABB2D(const GameEngineTransform& _boxA, const GameEngineTransform& _boxB)
{
	DirectX::BoundingBox boxA = _boxA.collisionDataObject_.aabb_;
	DirectX::BoundingBox boxB = _boxB.collisionDataObject_.aabb_;

	boxA.Center.z = 0.f;
	boxB.Center.z = 0.f;

	return boxA.Intersects(boxB);
}

bool GameEngineTransform::OBB2DToOBB2D(const GameEngineTransform& _boxA, const GameEngineTransform& _boxB)
{
	DirectX::BoundingOrientedBox boxA = _boxA.collisionDataObject_.obb_;
	DirectX::BoundingOrientedBox boxB = _boxB.collisionDataObject_.obb_;

	boxA.Center.z = 0.f;
	boxB.Center.z = 0.f;

	return boxA.Intersects(boxB);
}

bool GameEngineTransform::AABB2DToSphere2D(const GameEngineTransform& _box, const GameEngineTransform& _circle)
{
	DirectX::BoundingBox box = _box.collisionDataObject_.aabb_;
	DirectX::BoundingSphere circle = _circle.collisionDataObject_.sphere_;

	box.Center.z = 0.f;
	circle.Center.z = 0.f;

	return box.Intersects(circle);
}

bool GameEngineTransform::Sphere2DToAABB2D(const GameEngineTransform& _circle, const GameEngineTransform& _box)
{
	DirectX::BoundingSphere circle = _circle.collisionDataObject_.sphere_;
	DirectX::BoundingBox box = _box.collisionDataObject_.aabb_;

	circle.Center.z = 0.f;
	box.Center.z = 0.f;

	return circle.Intersects(box);
}

bool GameEngineTransform::OBB2DToSphere2D(const GameEngineTransform& _box, const GameEngineTransform& _circle)
{
	DirectX::BoundingOrientedBox box = _box.collisionDataObject_.obb_;
	DirectX::BoundingSphere circle = _circle.collisionDataObject_.sphere_;

	box.Center.z = 0.f;
	circle.Center.z = 0.f;

	return box.Intersects(circle);
}

bool GameEngineTransform::Sphere2DToOBB2D(const GameEngineTransform& _circle, const GameEngineTransform& _box)
{
	DirectX::BoundingSphere circle = _circle.collisionDataObject_.sphere_;
	DirectX::BoundingOrientedBox box = _box.collisionDataObject_.obb_;

	circle.Center.z = 0.f;
	box.Center.z = 0.f;

	return circle.Intersects(box);
}

void GameEngineTransform::CollisionScaleSetting()
{
	collisionDataObject_.obb_.Extents = (data_.worldScaleVector_.ABS3DReturn() * 0.5f);
}

void GameEngineTransform::CollisionRotationSetting()
{
	collisionDataObject_.obb_.Orientation = data_.worldRotationVector_.DegreeRotationToQuarternionReturn();
}

void GameEngineTransform::CollisionPositionSetting()
{
	collisionDataObject_.obb_.Center = data_.worldPositionVector_;
}

void GameEngineTransform::CollisionDataSetting()
{
	CollisionScaleSetting();
	CollisionRotationSetting();
	CollisionPositionSetting();
}

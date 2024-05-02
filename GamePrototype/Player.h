#pragma once
#include "utils.h"

class Player
{
public:
	explicit Player(const Point2f& pos);

	void Draw() const;
	void Update(float elapsedSec, const Rectf& gameArea);
	void ApplyForce(const Vector2f& force);
	Point2f GetPosition() const;
	Circlef GetHitbox() const;
private:
	Point2f m_Position;
	Vector2f m_Direction;
	float m_Angle;
};


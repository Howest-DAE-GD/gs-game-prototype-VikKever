#pragma once
#include "utils.h"

class Player
{
public:
	explicit Player(const Point2f& pos);

	void Draw() const;
	void Update(float elapsedSec, const Rectf& gameArea, float playerSpeed);
	void ApplyForce(const Vector2f& force);
	void Stun();
	bool IsStunned() const;
	Point2f GetPosition() const;
	void SetPosition(const Point2f& pos);
	Circlef GetHitbox() const;
private:
	Point2f m_Position;
	Vector2f m_Direction;
	float m_Angle;

	float m_StunnedTimer;

	float m_SpeedPercentage;
};


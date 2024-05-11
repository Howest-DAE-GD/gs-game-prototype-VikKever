#include "pch.h"
#include "Player.h"
#include <iostream>

Player::Player(const Point2f& pos)
	:m_Position{pos}
	, m_Angle{0.f}
	, m_Direction{0, 0}
{
}

void Player::Draw() const
{
	const float size{ 15.f };
	const float arrowAngle{ 3 * M_PI / 4 };
	utils::SetColor(Color4f{ 1.f, 0.5f, 0.f, 1.f });
	const Point2f p1{ m_Position.x + cosf(m_Angle) * size, m_Position.y + sinf(m_Angle) * size};
	const Point2f p2{ m_Position.x + cosf(m_Angle + arrowAngle) * size, m_Position.y + sinf(m_Angle + arrowAngle) * size};
	const Point2f p3{ m_Position.x + cosf(m_Angle - arrowAngle) * size, m_Position.y + sinf(m_Angle - arrowAngle) * size};
	utils::FillTriangle(p1, p2, p3);
}

void Player::Update(float elapsedSec, const Rectf& gameArea, float playerSpeed)
{
	//std::cout << m_Direction.Length() << std::endl;

	m_Position += m_Direction.Normalized() * playerSpeed * elapsedSec;

	if (m_Position.x < gameArea.left)
	{
		m_Position.x = gameArea.left;
		m_Direction.x *= -1;
	}
	else if (m_Position.x > gameArea.left + gameArea.width)
	{
		m_Position.x = gameArea.left + gameArea.width;
		m_Direction.x *= -1;
	}
	if (m_Position.y < gameArea.bottom)
	{
		m_Position.y = gameArea.left;
		m_Direction.y *= -1;
	}
	else if (m_Position.y > gameArea.bottom + gameArea.height)
	{
		m_Position.y = gameArea.bottom + gameArea.height;
		m_Direction.y *= -1;
	}

	m_Angle = atan2f(m_Direction.y, m_Direction.x);
	m_Direction = m_Direction * pow(0.1, elapsedSec);
}

void Player::ApplyForce(const Vector2f& force)
{
	m_Direction += force;
}

Point2f Player::GetPosition() const
{
	return m_Position;
}

Circlef Player::GetHitbox() const
{
	return Circlef{ m_Position, 10.f };
}

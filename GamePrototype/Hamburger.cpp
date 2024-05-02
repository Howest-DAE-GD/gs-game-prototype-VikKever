#include "pch.h"
#include "Hamburger.h"

Hamburger::Hamburger(const Point2f& pos)
	:Item(pos)
{
}

void Hamburger::Draw() const
{
	const float size{ 15.f };
	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	utils::FillRect(m_Position.x - size / 2, m_Position.y - size / 2, size, size);
}

Vector2f Hamburger::GetForce(const Point2f& pos) const
{
	const float dst{ utils::GetDistance(pos, m_Position) };
	const float strength{ 100000.f / (dst * dst) };
	const Vector2f delta{ m_Position - pos };
	return delta.Normalized() * strength;
}

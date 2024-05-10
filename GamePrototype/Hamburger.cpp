#include "pch.h"
#include "Hamburger.h"

Hamburger::Hamburger(const Point2f& pos)
	:Item(pos)
{
}

void Hamburger::Draw() const
{
	const float size{ 20.f };
	const float openingSize{ 5.f };
	const float burgerSize{ 15.f };

	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	utils::FillRect(m_Position.x - burgerSize / 2, m_Position.y - openingSize / 2, burgerSize, openingSize);

	utils::SetColor(Color4f{ 1.f, 0.8f, 0.f, 1.f });
	utils::FillArc(m_Position.x, m_Position.y + openingSize / 2, size / 2, (size - openingSize) / 2, 0.f, M_PI);
	utils::FillRect(m_Position.x - size / 2, m_Position.y - size / 4, size, (size - openingSize) / 4);

}

Vector2f Hamburger::GetForce(const Point2f& pos) const
{
	const float dst{ utils::GetDistance(pos, m_Position) };
	const float strength{ 100000.f / (dst * dst) };
	const Vector2f delta{ m_Position - pos };
	return delta.Normalized() * strength;
}

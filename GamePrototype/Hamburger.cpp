#include "pch.h"
#include "Hamburger.h"

Hamburger::Hamburger(const Point2f& pos)
	:Item(pos)
	, m_Highlighted{true}
{
}

void Hamburger::Draw() const
{
	if (m_Highlighted) Draw(m_Position, 0.7f);
	else Draw(m_Position);
}

Vector2f Hamburger::GetForce(const Point2f& pos) const
{
	const float dst{ utils::GetDistance(pos, m_Position) };
	const float strength{ 100000.f / (dst * dst) };
	const Vector2f delta{ m_Position - pos };
	return delta.Normalized() * strength;
}

void Hamburger::SetHighlighted(bool setHightlighted)
{
	m_Highlighted = setHightlighted;
}

void Hamburger::Draw(const Point2f& pos, float opacity)
{
	const float size{ 30.f };
	const float openingSize{ 8.f };
	const float burgerSize{ 23.f };

	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, opacity });
	utils::FillRect(pos.x - burgerSize / 2, pos.y - openingSize / 2, burgerSize, openingSize);

	utils::SetColor(Color4f{ 1.f, 0.8f, 0.f, opacity });
	utils::FillArc(pos.x, pos.y + openingSize / 2, size / 2, (size - openingSize) / 2, 0.f, M_PI);
	utils::FillRect(pos.x - size / 2, pos.y - size / 4, size, (size - openingSize) / 4);
}

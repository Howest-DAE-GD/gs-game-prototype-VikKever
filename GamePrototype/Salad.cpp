#include "pch.h"
#include "Salad.h"
#include "utils.h"

Salad::Salad(const Point2f pos)
	:Item(pos)
{
}

void Salad::Draw() const
{
	const float bowlSize{ 20.f };
	const float saladHeight{ 10.f };
	const float saladWidth{ 15.f };
	utils::SetColor(Color4f{ 0.f, 0.7f, 0.f, 1.f });
	utils::FillEllipse(m_Position.x, m_Position.y, saladWidth / 2, saladHeight / 2);
	utils::SetColor(Color4f{ 0.8f, 0.8f, 0.9f, 1.f });
	utils::FillArc(m_Position.x, m_Position.y, bowlSize / 2, bowlSize / 2, -M_PI, 0.f);
}

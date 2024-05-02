#include "pch.h"
#include "Salad.h"
#include "utils.h"

Salad::Salad(const Point2f pos)
	:Item(pos)
{
}

void Salad::Draw() const
{
	const float size{ 15.f };
	utils::SetColor(Color4f{ 0.f, 0.7f, 0.f, 1.f });
	utils::FillEllipse(m_Position, size / 2, size / 2);
}

#include "pch.h"
#include "Item.h"
#include "utils.h"

Item::Item(const Point2f pos)
	:m_Position{ pos }
{
}

Circlef Item::GetHitbox() const
{
	return Circlef{ m_Position, 20.f };
}
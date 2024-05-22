#include "pch.h"
#include "Spike.h"
#include "utils.h"

Spike::Spike(const Point2f& pos)
	:Item(pos)
	, m_DespawnTimer{10.f}
{
}

void Spike::Draw() const
{
	const float bigSize{ 35.f };
	const float smallSize{ 20.f };
	const int numSpikes{ 9 };

	utils::SetColor(Color4f{ 0.6f, 0.f, 0.f, 1.f });
	for (int spike{}; spike < numSpikes; ++spike)
	{
		const float angle2{ spike / float(numSpikes) * float(M_PI * 2) };
		const float angle1{ angle2 - float(M_PI * 2) / (2 * numSpikes) };
		const float angle3{ angle2 + float(M_PI * 2) / (2 * numSpikes) };
		const Point2f p1{ m_Position.x + (smallSize / 2) * cosf(angle1), m_Position.y + (smallSize / 2) * sinf(angle1) };
		const Point2f p2{ m_Position.x + (bigSize / 2) * cosf(angle2), m_Position.y + (bigSize / 2) * sinf(angle2) };
		const Point2f p3{ m_Position.x + (smallSize / 2) * cosf(angle3), m_Position.y + (smallSize / 2) * sinf(angle3) };

		utils::FillTriangle(p1, p2, p3);
	}

	utils::FillEllipse(m_Position, smallSize / 2, smallSize / 2);
}

void Spike::UpdateTimer(float elapsedSec)
{
	m_DespawnTimer -= elapsedSec;
}

bool Spike::ShouldDespawn() const
{
	return m_DespawnTimer <= 0.f;
}

#pragma once
#include "Item.h"

class Spike : public Item
{
public:
	explicit Spike(const Point2f& pos);

	virtual void Draw() const override;
	void UpdateTimer(float elapsedSec);
	bool ShouldDespawn() const;
private:
	float m_DespawnTimer;
};


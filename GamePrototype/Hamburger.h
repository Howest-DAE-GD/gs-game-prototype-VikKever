#pragma once
#include "utils.h"
#include "Item.h"

class Hamburger final: public Item
{
public:
	explicit Hamburger(const Point2f& pos);

	void Draw() const override;
	Vector2f GetForce(const Point2f& pos) const;
};


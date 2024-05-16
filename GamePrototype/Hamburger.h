#pragma once
#include "utils.h"
#include "Item.h"

class Hamburger final: public Item
{
public:
	explicit Hamburger(const Point2f& pos);

	void Draw() const override;
	Vector2f GetForce(const Point2f& pos) const;
	void SetHighlighted(bool setHightlighted);

	static void Draw(const Point2f& pos, float opacity = 1.f);
private:
	bool m_Highlighted;
};


#pragma once
#include "Item.h"

class Salad final: public Item
{
public:
	explicit Salad(const Point2f pos);
	
	void Draw() const override;
};


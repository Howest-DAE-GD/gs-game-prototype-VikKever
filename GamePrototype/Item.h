#pragma once
class Item
{
public:
	explicit Item(const Point2f pos);
	virtual ~Item() = default;

	virtual void Draw() const = 0;
	Circlef GetHitbox() const;
protected:
	Point2f m_Position;
};


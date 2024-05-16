#pragma once
class Item
{
public:
	explicit Item(const Point2f pos);
	virtual ~Item() = default;

	virtual void Draw() const = 0;
	Circlef GetHitbox() const;
	Circlef GetClickArea() const;
protected:
	Point2f m_Position;
};


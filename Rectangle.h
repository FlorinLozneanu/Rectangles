#pragma once

#include <iostream>

typedef size_t rect_index_t;

struct Segment
{
	int a;
	int b;
	rect_index_t rectIndex1;
	rect_index_t rectIndex2;

	Segment()
		: a(INT_MIN)
		, b(INT_MAX)
		, rectIndex1(-1)
		, rectIndex2(-1)
	{

	}

	Segment(int a1, int b1)
		: a(a1)
		, b(b1)
	{

	}

	bool empty() const
	{
		return (a == b) || ((a == INT_MIN) && (b == INT_MAX));
	}

	bool intersect(const Segment &other) const
	{
		return !((a >= other.b) || (b <= other.a));
	}

	Segment getIntersection(const Segment &other) const
	{
		return { std::max(a, other.a), std::min(b, other.b) };
	}

	Segment findIntersection(const Segment &other) const
	{
		if (!intersect(other))
			return Segment();

		return getIntersection(other);
	}

	// Overrides the 'less' operator
	bool operator <(const Segment &other) const
	{
		return a < other.a;
	}
};

class Rectangle
{
public:
	Rectangle()
		: m_index(-1)
		, m_left(0)
		, m_top(0)
		, m_right(0)
		, m_bottom(0)
	{

	}

	virtual ~Rectangle() {}

	Rectangle(int left, int top, int width, int height)
	{
		m_left = left;
		m_top = top;
		m_right = m_left + width;
		m_bottom = m_top + height;
	}

	Rectangle(const Rectangle &other)
	{
		m_index = other.m_index;
		m_left = other.m_left;
		m_right = other.m_right;
		m_top = other.m_top;
		m_bottom = other.m_bottom;
	}

	Rectangle& operator =(const Rectangle &other)
	{
		if (&other == this)
			return *this;

		m_index = other.m_index;
		m_left = other.m_left;
		m_right = other.m_right;
		m_top = other.m_top;
		m_bottom = other.m_bottom;

		return *this;
	}
	
	void set(int left, int top, int width, int height)
	{
		m_left = left;
		m_top = top;
		m_right = m_left + width;
		m_bottom = m_top + height;
	}

	void setTopLeft(int left, int top)
	{
		m_left = left;
		m_top = top;
	}

	void setRightBottom(int right, int bottom)
	{
		m_right = right;
		m_bottom = bottom;
	}

	rect_index_t index() const
	{
		return m_index;
	}

	void setIndex(rect_index_t index)
	{
		m_index = index;
	}

	void setLeft(int left)
	{
		m_left = left;
	}

	int left() const
	{
		return m_left;
	}

	void setTop(int top)
	{
		m_top = top;
	}

	int top() const
	{
		return m_top;
	}

	void setRight(int right)
	{
		m_right = right;
	}

	int right() const
	{
		return m_right;
	}

	void setBottom(int bottom)
	{
		m_bottom = bottom;
	}

	int bottom() const
	{
		return m_bottom;
	}

	int width() const
	{
		return m_right - m_left;
	}

	int height() const
	{
		return m_bottom - m_top;
	}

	bool empty() const
	{
		return (width() == 0) || (height() == 0);
	}

	// Tests if 'this' rectangle overlaps another rectangle.
	bool intersect(const Rectangle &rect) const
	{
		bool noOverlap = (m_left > rect.m_right) || (rect.m_left > m_right) ||
			(m_top > rect.m_bottom) || (rect.m_top > m_bottom);

		return !noOverlap;
	}

	// Finds the intersection between 'this' rectangle and a another one.
	Rectangle findIntersection(const Rectangle &rect) const
	{
		Rectangle result;
		if (intersect(rect))
		{
			result = getIntersection(rect);
		}
		return result;
	}

	//
	Segment findIntersectionWithVertSegment(const Rectangle &rect) const
	{
		Segment segment;
		if (intersect(rect))
		{
			segment.a = std::max<int>(m_top, rect.m_top);
			segment.b = std::min<int>(m_bottom, rect.m_bottom);
			segment.rectIndex1 = index();
			segment.rectIndex2 = rect.index();
		}
		return segment;
	}

	//
	Rectangle getIntersection(const Rectangle &rect) const
	{
		Rectangle result;

		result.setTopLeft(std::max<int>(m_left, rect.m_left), std::max<int>(m_top, rect.m_top));
		result.setRightBottom(std::min<int>(m_right, rect.m_right), std::min<int>(m_bottom, rect.m_bottom));

		return result;
	}

	friend std::ostream& operator <<(std::ostream &os, const Rectangle &rect)
	{
		os << "(" << rect.left() << ", " << rect.top() << "), "
			<< "w=" << rect.width() << ", h=" << rect.height() << ".";
		return os;
	}

protected:
	rect_index_t m_index;
	int m_left;
	int m_top;
	int m_right;
	int m_bottom;
};

inline bool operator ==(const Rectangle &lhs, const Rectangle &rhs)
{
	return (lhs.left() == rhs.left()) && (lhs.top() == rhs.top()) &&
		(lhs.right() == rhs.right()) && (lhs.bottom() == rhs.bottom());
}
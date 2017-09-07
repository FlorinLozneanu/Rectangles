#pragma once

#include "Rectangle.h"

enum EventType : int
{
	In,
	Out
};

struct Event
{
	rect_index_t rectIndex;	// index of the rectangle
	int x;					// the X-coordinate of the left or right edge of the rectangle
	EventType type;			// the type of the event  (In: left edge, Out: right edge)

	// Overrides the 'less' operator (it will be used by std::sort).
	bool operator <(const Event &other) const
	{
		return x < other.x;
	}
};

struct OveralappingRectangles
{
	// the set of rectangles which intersect each other (I choose 'set' because I want to print them in ascending order).
	std::set<rect_index_t> rectIndexes;

	// the intersection between all the rectangles from 'rectIndexes'
	Rectangle overlapRect;

	OveralappingRectangles() {}

	OveralappingRectangles(const std::set<rect_index_t> indexes, const Rectangle &rect)
		: rectIndexes(indexes)
		, overlapRect(rect)
	{

	}
};

///
/// An instance of the class OverlappedRectsSolver receives a collection of rectangles
/// and find all the intersections between two or more rectangles.
///
class OverlappedRectsSolver
{
public:
	// The default constructor.
	OverlappedRectsSolver() {}

	// Constructor : instantiates an instance of this class and sets the collection of the rectangles.
	OverlappedRectsSolver(const std::vector<Rectangle>& rectangles);

	// Destructor.
	~OverlappedRectsSolver();

	void setRectangles(const std::vector<Rectangle>&& rectangles)
	{
		m_rectangles.clear();
		m_rectangles = std::move(rectangles);
	}

	// Finds all the intersections between two or more rectangles.
	void solve();

	// Prints the intersections between the rectangles from the current collection.
	void printResults();

	typedef std::list<OveralappingRectangles> OverlappingRectList;
	typedef std::map<rect_index_t, OverlappingRectList> Results;

	// Returns the results.
	const Results& results() const
	{
		return m_results;
	}

private:
	void prepare();

	// Checks the intersaction between the given rectangle 'rect' and the set of the active rectangles.
	// Collects the intersection segments in the output parameter 'segments'.
	bool checkIntersection(const Rectangle& rect, std::vector<Segment> &segments);

	// Process the collection of the intersection segments and find out all the intersections between rectangles. 
	void processIntersectionSegments(const std::vector<Segment> &segments);

	// Updates the results (overlapping rectangles).
	// param 'key': the number of the rectangles which overlap (2, 3, 4 etc)
	// param 'rects' : the overlapping rectangles to be added to the current results
	void updateResults(size_t key, const OveralappingRectangles& rects);

private:
	std::vector<Rectangle> m_rectangles;			// the collection of the rectangles
	std::vector<Event> m_events;					// the events generated from m_rectangles
	std::set<rect_index_t> m_activeRects;			// the active rectangles

	// The results grouped by the number of the overlapping rectangles:
	// key = 2 : all the pairs of the overlapping rectangles
	// key = 3 : all the intersections between three rectangles
	// key = 4 : all the intersections between four rectangles
	// ... and so on
	Results m_results;
};
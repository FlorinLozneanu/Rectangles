
#include "stdafx.h"
#include "OverlappedRectsSolver.h"

///
OverlappedRectsSolver::OverlappedRectsSolver(const std::vector<Rectangle>& rects)
	: m_rectangles(rects)
{
}

///
OverlappedRectsSolver::~OverlappedRectsSolver()
{
}

///
/// Initializes the set of the active rectangles, generates the array of events and sorts the latter.
///
void OverlappedRectsSolver::prepare()
{
	m_activeRects.reserve(m_rectangles.size());

	m_events.clear();
	m_events.reserve(2 * m_rectangles.size());

	for (auto &rect : m_rectangles)
	{
		m_events.push_back({ rect.index(), rect.left(), EventType::In });
		m_events.push_back({ rect.index(), rect.right(), EventType::Out });
	}

	std::sort(m_events.begin(), m_events.end());	// O(n*log(n))
}

///
/// Searches for the overlapped rectangles.
///
/// The approach is inspired from the sweep-line algorithm described in paper:
///	https://users.cs.duke.edu/~edels/Papers/1983-J-05-RectangleIntersections:PartII.pdf
///
void OverlappedRectsSolver::solve()
{
	prepare();

	for (auto& event : m_events)
	{
		if (event.type == EventType::In)
		{
			// Gets the rectangle where generated event belongs to.
			const Rectangle& newActiveRect = m_rectangles[event.rectIndex];

			// Checks if the current rectangle overlaps with existing 'active' rectangles and
			// collects the vertical (Y coordinates) intersection segments.
			std::vector<Segment> segments;
			if (checkIntersection(newActiveRect, segments))
			{
				std::sort(segments.begin(), segments.end());
				processIntersectionSegments(segments);
			}

			// Inserts the index into the set of active rectangles
			m_activeRects.insert(newActiveRect.index());
		}
		else
		{
			// 'Right edge' event: removes the index from the set of the 'active' rectangles.
			m_activeRects.erase(event.rectIndex);
		}
	}
}

///
void OverlappedRectsSolver::processIntersectionSegments(const std::vector<Segment> &segments)
{
	for (size_t i = 0; i < segments.size(); i++)
	{
		Rectangle& rect = m_rectangles[segments[i].rectIndex1];
		Rectangle rect2 = m_rectangles[segments[i].rectIndex2];
		Rectangle overlap = rect.getIntersection(rect2);

		OveralappingRectangles ovlRects;
		ovlRects.rectIndexes = std::set<size_t>({ segments[i].rectIndex1, segments[i].rectIndex2 });
		ovlRects.overlapRect = overlap;
		updateResults(2, ovlRects);

		std::set<size_t> rects; // I'm using 'set' just for having the rectangle indexes printed in ascending order
		rects.insert(segments[i].rectIndex1);
		rects.insert(segments[i].rectIndex2);

		for (size_t j = i + 1; j < segments.size(); j++)
		{
			if (segments[i].intersect(segments[j]))
			{
				rects.insert(segments[j].rectIndex2);
				overlap = overlap.getIntersection(m_rectangles[segments[j].rectIndex2]);
			}
			else
			{
				break;
			}
		}

		if (rects.size() > 2)
		{
			// do we have more than two overlapping rectangles ?
			OveralappingRectangles ovlRects;
			ovlRects.rectIndexes = rects;
			ovlRects.overlapRect = overlap;
			updateResults(rects.size(), ovlRects);
		}
	}
}

///
void OverlappedRectsSolver::updateResults(size_t key, const OveralappingRectangles& rects)
{
	auto it = m_results.find(key);
	if (it == m_results.end())
	{
		auto res = m_results.insert(std::make_pair(key, std::list<OveralappingRectangles>()));
		it = res.first;
	}
	it->second.push_back(rects);
}

///
void OverlappedRectsSolver::printResults()
{
	std::cout << "Intersections" << std::endl;

	for (auto& kv : m_results)
	{
		for (auto& result : kv.second)
		{
			std::cout << "Between rectangles ";
			std::stringstream stream;
			for (auto i : result.rectIndexes)
			{
				stream << (i+1) << ", ";
			}
			auto s = stream.str();

			// remove the last ',' in string
			s = s.substr(0, s.length() - 2);

			// replaces the last ',' with 'and' and print the string to stdout
			auto pos = s.find_last_of(',');
			if (pos != std::string::npos)
			{
				s.replace(pos, 1, " and");
			}
			std::cout << s.c_str();

			std::cout << " at " << result.overlapRect << std::endl;
		}
	}
}

///
/// Checks the intersaction between the given rectangle 'rect' and the set of the active rectangles.
/// Collects the intersection segments in the output parameter 'segments'.
///
bool OverlappedRectsSolver::checkIntersection(const Rectangle& rect, std::vector<Segment> &segments)
{
	for (auto& activeRectIndex : m_activeRects)
	{
		auto segment = rect.findIntersectionWithVertSegment(m_rectangles[activeRectIndex]);
		if (!segment.empty())
		{
			segments.push_back(segment);
		}
	}
	return !segments.empty();
}

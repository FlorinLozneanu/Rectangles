// Tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gtest\gtest.h"

#include "..\Rectangles.h"
#include "..\OverlappedRectsSolver.h"

TEST(Rectangles, WrongJsonFilePath) {
	Rectangles rects;
	auto res = rects.loadFromFile("C:\\Windows\\rects.json");
	ASSERT_EQ(res, Rectangles::Status::CannotOpenFile);
}

TEST(SquareRootTest, InvalidJsonFormat) {
	Rectangles rects;
	auto res = rects.loadFromFile("..\\data\\invalid-format.json");
	ASSERT_EQ(res, Rectangles::Status::UnexpectedJsonObject);

}

TEST(Rectangles, InvalidJsonFormat_NegativeValues)
{
	Rectangles rects;
	auto res = rects.loadFromFile("..\\data\\sample-negative-values.json");
	ASSERT_EQ(res, Rectangles::Status::InvalidRectValues);
}

TEST(Rectangles, Sample1)
{
	Rectangles rects;
	auto res = rects.loadFromFile("..\\data\\sample1.json");
	ASSERT_EQ(res, Rectangles::Status::Ok);
	ASSERT_EQ(rects.count(), 4);

	OverlappedRectsSolver solver(rects.rectangles());
	solver.solve();

	auto& results = solver.results();
	ASSERT_EQ(results.size(), 2);

	auto& it = results.find(2);
	ASSERT_NE(it, results.end());
	ASSERT_EQ(it->second.size(), 5);

	it = results.find(3);
	ASSERT_NE(it, results.end());
	ASSERT_EQ(it->second.size(), 2);
}

TEST(Rectangles, Sample2)
{
	Rectangles rects;
	auto res = rects.loadFromFile("..\\data\\sample2.json");
	ASSERT_EQ(res, Rectangles::Status::Ok);
	ASSERT_EQ(rects.count(), 6);

	OverlappedRectsSolver solver(rects.rectangles());
	solver.solve();

	auto& results = solver.results();
	ASSERT_EQ(results.size(), 5);

	auto& it = results.find(2);
	ASSERT_NE(it, results.end());
	ASSERT_EQ(it->second.size(), 15);

	it = results.find(3);
	ASSERT_NE(it, results.end());
	ASSERT_EQ(it->second.size(), 4);

	it = results.find(4);
	ASSERT_NE(it, results.end());
	ASSERT_EQ(it->second.size(), 3);

	it = results.find(5);
	ASSERT_NE(it, results.end());
	ASSERT_EQ(it->second.size(), 2);

	it = results.find(6);
	ASSERT_NE(it, results.end());
	ASSERT_EQ(it->second.size(), 1);
}

TEST(Rectangles, Sample3_NoIntersection)
{
	Rectangles rects;
	auto res = rects.loadFromFile("..\\data\\sample3.json");
	ASSERT_EQ(res, Rectangles::Status::Ok);
	ASSERT_EQ(rects.count(), 3);

	OverlappedRectsSolver solver(rects.rectangles());
	solver.solve();

	auto& results = solver.results();
	ASSERT_EQ(results.size(), 0);
}

TEST(Rectangles, Sample_SingleRect)
{
	Rectangles rects;
	auto res = rects.loadFromFile("..\\data\\single-rect.json");
	ASSERT_EQ(res, Rectangles::Status::Ok);
	ASSERT_EQ(rects.count(), 1);

	OverlappedRectsSolver solver(rects.rectangles());
	solver.solve();

	auto& results = solver.results();
	ASSERT_EQ(results.size(), 0);
}

/// Unit tests for class Segment
TEST(Segment, Intersection)
{
	Segment seg1{ 3, 8 };
	Segment seg2{ 6, 24 };
	ASSERT_EQ(seg1.intersect(seg2), true);
}

TEST(Segment, Intersection2)
{
	Segment seg1{ 7, 14 };
	Segment seg2{ 3, 10 };
	ASSERT_EQ(seg1.intersect(seg2), true);
}

TEST(Segment, Intersection3)
{
	Segment seg1{ 6, 18 };
	Segment seg2{ 10, 14 };
	ASSERT_EQ(seg1.intersect(seg2), true);
}

TEST(Segment, Intersection4)
{
	Segment seg1{ 6, 18 };
	Segment seg2{ 6, 18 };
	ASSERT_EQ(seg1.intersect(seg2), true);
}

TEST(Segment, IntersectionNot)
{
	Segment seg1{ 6, 18 };
	Segment seg2{ 20, 24 };
	ASSERT_EQ(seg1.intersect(seg2), false);
}

TEST(Segment, IntersectionNot2)
{
	Segment seg1{ 6, 18 };
	Segment seg2{ 1, 4 };
	ASSERT_EQ(seg1.intersect(seg2), false);
}

///
TEST(Segment, GetIntersection)
{
	Segment seg1{ 3, 8 };
	Segment seg2{ 6, 24 };
	Segment seg = seg1.getIntersection(seg2);
	ASSERT_EQ((seg.a == 6) && (seg.b == 8), true);
}

TEST(Segment, GetIntersection2)
{
	Segment seg1{ 7, 14 };
	Segment seg2{ 3, 10 };

	Segment seg = seg1.getIntersection(seg2);
	ASSERT_EQ((seg.a == 7) && (seg.b == 10), true);
}

TEST(Segment, GetIntersection3)
{
	Segment seg1{ 6, 18 };
	Segment seg2{ 10, 14 };
	Segment seg = seg1.getIntersection(seg2);
	ASSERT_EQ((seg.a == 10) && (seg.b == 14), true);
}

TEST(Segment, GetIntersection4)
{
	Segment seg1{ 6, 18 };
	Segment seg2{ 6, 18 };
	Segment seg = seg1.getIntersection(seg2);
	ASSERT_EQ((seg.a == 6) && (seg.b == 18), true);
}

TEST(Segment, FindIntersectionNot)
{
	Segment seg1{ 6, 18 };
	Segment seg2{ 20, 24 };
	Segment seg = seg1.findIntersection(seg2);
	ASSERT_EQ(seg.empty(), true);
}

TEST(Segment, FindIntersectionNot2)
{
	Segment seg1{ 6, 18 };
	Segment seg2{ 1, 4 };
	Segment seg = seg1.findIntersection(seg2);
	ASSERT_EQ(seg.empty(), true);
}

///
TEST(Rectangle, FindIntersectionWithSegment)
{
	Rectangle rect{ 10, 10, 20, 30 };	// (10, 10), (30, 40)
	Rectangle rect2{ 5, 15, 10, 20 };	// (5, 15), (15, 35}
	auto seg2 = rect.findIntersectionWithVertSegment(rect2);
	ASSERT_EQ(seg2.empty(), false);
	ASSERT_EQ((seg2.a == 15) && (seg2.b == 35), true);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}



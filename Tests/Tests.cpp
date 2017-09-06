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

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}



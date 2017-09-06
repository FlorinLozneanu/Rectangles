#pragma once

#include "Rectangle.h"
#include "json\json.hpp"

namespace json = nlohmann;

class Rectangles
{
public:

	enum Status : int
	{
		Ok = 0,
		CannotOpenFile,
		InvalidJsonFormat,
		UnexpectedJsonObject,
		InvalidRectValues,
	};

	Rectangles();
	~Rectangles();

	Status loadFromFile(const std::string &filePath);
	void printRectangles();
	void solveOverlappedRects();

	size_t count() const
	{
		return m_rectangles.size();
	}

	const std::vector<Rectangle> rectangles() const
	{
		return m_rectangles;
	}

	Status fromJson(const json::json &);

private:
	bool isValidJsonRectObject(const json::json &);

private:
	std::vector<Rectangle> m_rectangles;
};

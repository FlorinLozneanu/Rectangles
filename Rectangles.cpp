
#include "stdafx.h"

#include "Rectangles.h"
#include "OverlappedRectsSolver.h"
#include "json\json.hpp"

namespace json = nlohmann;

static const int kErrorMessageLen = 512;
static const int kMaxRectangleCount = 1000;

///
Rectangles::Rectangles()
{
	m_rectangles.reserve(kMaxRectangleCount);
}

///
Rectangles::~Rectangles()
{

}

///
Rectangles::Status Rectangles::loadFromFile(const std::string &filePath)
{
	try
	{
		std::ifstream in(filePath, std::wifstream::in);
		if (!in)
		{
			std::string msg;
			msg.reserve(kErrorMessageLen);
			strerror_s(const_cast<char*>(msg.data()), msg.capacity(), errno);

			std::cerr << "Failed to open file " << filePath.c_str() 
				<< ", error: " << msg.c_str() << std::endl;

			return CannotOpenFile;
		}

		// load the JSON content from file
		json::json content;
		in >> content;

		return fromJson(content);
	}
	catch (std::exception &e)
	{
		std::cerr << "Exception caught in file " << __FILE__
			<< " at line " << __LINE__
			<< " : " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Unknown exception caught in file " << __FILE__
			<< " at line " << __LINE__ << std::endl;
	}

	return InvalidJsonFormat;
}

///
Rectangles::Status Rectangles::fromJson(const json::json &js)
{
	try
	{
		// searches for the array object which stores the rectangles
		auto root = js.find("rects");
		if (root == js.end())
		{
			std::cerr << "The key 'rects' was not found in JSON object" << std::endl;
			return UnexpectedJsonObject;
		}

		size_t counter = 0;

		// starts iterating over the array of the rectangles
		for (auto it = root->begin(); it != root->end(); ++it)
		{
			// validate the current JSON object
			if (!isValidJsonRectObject(*it))
			{
				std::cerr << "The JSON object is not a 'Rectangle':" << std::endl;
				std::cerr << it->dump().c_str() << std::endl;
				return UnexpectedJsonObject;
			}

			auto x = it->at("x").get<int>();
			auto y = it->at("y").get<int>();
			auto w = it->at("w").get<int>();
			auto h = it->at("h").get<int>();

			//
			if ((w < 0) || (y < 0))
			{
				std::cerr << "The dimensions of the rectangle should be positive values:" << std::endl;
				std::cerr << it->dump().c_str() << std::endl;
				return InvalidRectValues;
			}

			// instantiates a Rectangle object and stores it into the collection of rectangles
			Rectangle r;
			r.set(it->at("x").get<int>(), it->at("y").get<int>(),
				it->at("w").get<int>(), it->at("h").get<int>());

			r.setIndex(counter);
			m_rectangles.push_back(r);

			if (++counter >= kMaxRectangleCount)
				break;
		}

		return Ok;
	}
	catch (std::exception &e)
	{
		std::cerr << "Exception caught in file " << __FILE__ 
			<< " at line " << __LINE__ 
			<< " : " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Unknown exception caught in file " << __FILE__ 
			<< " at line " << __LINE__ << std::endl;
	}
	return InvalidJsonFormat;
}

///
bool Rectangles::isValidJsonRectObject(const json::json &js)
{
	auto end = js.end();
	return (js.find("x") != end) && ((js.find("y") != end))
		&& ((js.find("w") != end)) && (js.find("h") != end);
}

///
void Rectangles::printRectangles()
{
	auto counter = 1;
	for (auto& rect: m_rectangles)
	{
		std::cout << (counter++) << ": " << rect << std::endl;
	}
}

void Rectangles::solveOverlappedRects()
{
	OverlappedRectsSolver solver(m_rectangles);
	solver.solve();
	solver.printResults();
}


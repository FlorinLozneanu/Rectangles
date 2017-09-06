#include "stdafx.h"
#include "Rectangles.h"

///
int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cout << "Usage:\nrectangles.exe json_file_path" << std::endl;
		exit(1);
	}

	Rectangles rects;
	auto error = rects.loadFromFile(argv[1]);
	if (error != Rectangles::Status::Ok)
	{
		std::cerr << "Failed to load the JSON from file " << argv[1] 
			<< ", error = " << error << std::endl;
		exit(1);
	}

	rects.printRectangles();
	rects.solveOverlappedRects();

	return 0;
}

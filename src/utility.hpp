#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cstdlib>
#include <vector>
#include <utility>

std::vector<std::pair<int, int>> DrawLine(int x1, int y1, int x2, int y2) {
	std::vector<std::pair<int, int>> points;
	bool issteep = std::abs(y2-y1) > std::abs(x2-x1);
	if (issteep == true) {
		int temp = x1;
		x1 = y1;
		y1 = temp;

		temp = x2;
		x2 = y2;
		y2 = temp;
	}

	bool rev = false;
	if (x1 > x2) {
		int temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
		rev = true;
	}
		
	int deltax = x2 - x1;
	int deltay = std::abs(y2-y1);
	int error = static_cast<int>(deltax / 2);
	int y = y1;
	int ystep;
	if (y1 < y2) {
		ystep = 1;
	} else {
		ystep = -1;
	}    
  
	for (int x = x1; x <= x2; x++) {
		if (issteep == true) {
			points.push_back(std::pair<int, int>(y, x));
		} else {
			points.push_back(std::pair<int, int>(x, y));
		}
		error -= deltay;
		if (error < 0) {
			y += ystep;
			error += deltax;
		}
	}

	//Reverse the list if the coordinates were reversed
	if (rev == true) {
		std::reverse(points.begin(), points.end());
	}
		
	return points;
}

/*bool LineCollision(std::vector<std::pair<int, int>>& line, std::vector<std::vector<int>>& collisionMap){
  for(auto& entry : line) {
    if(collisionMap[entry.second][entry.first] == 1){
      return entry;
    }else {
      return false;
    }
  }

}*/
#endif //UTILITY_HPP
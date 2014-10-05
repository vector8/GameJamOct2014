#include "Utilities.h"
#include <string>
#include <vector>
#include <glm\glm.hpp>
#include <math.h>
#include <sstream>

namespace v8g
{
	sf::Vector2f normalize(const sf::Vector2f &v)
	{
		if (compareFloats(v.x, 0.f) && compareFloats(v.y, 0.f))
			return v;
		else
			return (v / sqrtf(v.x * v.x + v.y * v.y));
	}
	
	float length(const sf::Vector2f &v)
	{
		return sqrtf(v.x * v.x + v.y * v.y);
	}

	float lengthSquared(const sf::Vector2f &v)
	{
		return v.x * v.x + v.y * v.y;
	}

	std::string ltrim(std::string s)
	{
		size_t startIndex;
		std::string result;

		if (s.size() == 0)
			result = s;
		else
		{
			startIndex = s.find_first_not_of(" \n\r\t");

			if (startIndex == std::string::npos)
				result = "";
			else
				result = s.substr(startIndex);
		}

		return result;
	}

	std::string rtrim(std::string s)
	{
		size_t endIndex;
		std::string result;

		if (s.size() == 0)
			result = s;
		else
		{
			endIndex = s.find_last_not_of(" \n\r\t");

			if (endIndex == std::string::npos)
				result = "";
			else
				result = s.substr(0, endIndex + 1);
		}

		return result;
	}

	std::string trim(std::string s)
	{
		return ltrim(rtrim(s));
	}

	std::vector<std::string> split(std::string s, std::string delimiters, bool shouldTrim)
	{
		std::vector<std::string> result;
		std::string temp;

		size_t start = 0;
		size_t end = s.find_first_of(delimiters);

		while (end != std::string::npos)
		{
			temp = s.substr(start, end - start);

			if (shouldTrim)
				temp = trim(temp);

			if (temp.size() > 0)
				result.push_back(temp);

			start = end + 1;
			end = s.find_first_of(delimiters, start);
		}

		// get the last token...
		temp = s.substr(start);

		if (shouldTrim)
			temp = trim(temp);

		if (temp.size() > 0)
			result.push_back(temp);

		return result;
	}

	int stringToInt(std::string s)
	{
		int result;

		std::stringstream ss(s);

		assert((ss >> result));

		return result;
	}

	std::string intToString(int i)
	{
		std::stringstream ss;

		ss << i;

		return ss.str();
	}

	int rollNumber(int lowerBound, int upperBound)
	{
		static bool first = true;

		if (first)
		{
			srand((unsigned)time(NULL));
			first = false;
		}

		return rand() % (upperBound - lowerBound) + lowerBound;
	}

	float rollNumber(float lowerBound, float upperBound, int precision)
	{
		static bool first = true;

		if (first)
		{
			srand((unsigned)time(NULL));
			first = false;
		}

		int power = pow(10, precision);

		int lowerInt = lowerBound * power;
		int upperInt = upperBound * power;

		int random = rand() % (upperInt - lowerInt) + lowerInt;

		return (float)random / (float)power;
	}
}

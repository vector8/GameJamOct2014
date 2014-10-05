#pragma once
#include <string>
#include <vector>
#include <SFML\System.hpp>

namespace v8g
{
	inline bool compareFloats(float first, float second, float tolerance = 0.0001f)
	{
		return abs(first - second) <= tolerance;
	}
	
	sf::Vector2f normalize(const sf::Vector2f &v);

	float length(const sf::Vector2f &v);
	
	float lengthSquared(const sf::Vector2f &v);

	template <class T>
	T lerp(float u, const T &p0, const T &p1)
	{
		return p0 * (1.f - u) + p1 * u;
	}

	template <class T>
	T catmull(float u, const T &p0, const T &p1, const T &p2, const T &p3)
	{
		return 0.5f * ((u * u * u) * (-p0 + 3.f * p1 - 3.f * p2 + p3) +
			(u * u) * (2.f * p0 - 5.f * p1 + 4.f * p2 - p3) +
			(u)* (-p0 + p2) +
			(2.f * p1));
	}

	std::string ltrim(std::string s);
	std::string rtrim(std::string s);
	std::string trim(std::string s);

	std::vector<std::string> split(std::string s, std::string delimiters, bool shouldTrim = true);

	int stringToInt(std::string s);
	std::string intToString(int i);

	int rollNumber(int lowerBound, int upperBound);

	float rollNumber(float lowerBound, float upperBound, int precision);
}

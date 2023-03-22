/* Copyright (c) 2013 Kevin L. Stern
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include <chrono>
#include <cstdint>
#include <functional>

struct Point {
	int32_t x;
	int32_t y;
	Point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	Point():Point(0,0)
	{

	}
};
// GeoIndex is an index for GeoPoints within CoordinateSystem that provides fast range queries.
class GeoHash {
public:

	static uint64_t encode(const Point p1)
	{
		return encode(p1.x, p1.y);
	}
	static uint64_t encode(const int32_t x, const int32_t y)
	{
		uint64_t uid = 0;
		int x_bit; 
		int y_bit;
		for (int i =0; i < 32; ++i)
		{
			uid <<= 1;
			x_bit = (x >> (31 - i))&0x1;
			uid |= x_bit;
			uid <<= 1;
			y_bit = (y >> (31 - i))&0x1;
			uid |= y_bit;

		}
		return uid;
	}


	

	static Point decode(const uint64_t uid)
	{
		int bit;
		int x = 0; 
		int y = 0;
		for (int i = 0; i < 32; ++i)
		{
			bit =(uid >>  ((31 - i) * 2)) & 0x3;
			x <<= 1;
			y <<= 1;
			x |= (bit >> 1);
			y |= (bit & 0x1);
		}

		Point p1(x, y);
		return p1;
	}


};

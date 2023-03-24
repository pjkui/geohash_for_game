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

#include <set>
#include <map>
#include <limits>

#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
// #ifdef NDEBUG
#include <chrono>
// #endif
struct ScopeProfiling
{
	std::string info;
	std::chrono::system_clock::time_point startTime;
	ScopeProfiling(std::string str)
	{
		info = str;
		
		startTime = std::chrono::system_clock::now();
	}
	~ScopeProfiling()
	{
		auto endTime = std::chrono::system_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime-startTime).count();
		std::cout <<"Time Cost:" << info <<  duration/1000.f<<"ms"<< std::endl;
	}
};
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
	Point operator - (const Point& Other) const
	{
		return {x - Other.x, y - Other.y};
	}
	bool operator ==(const Point& Other)
	{
		return x == Other.x && y == Other.y;
	}
	void Log() const
	{
		std::cout << "(" << x << ", " << y << ") ";
	 }


	void reset(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	float Dist(const Point& Other) const
	{
		const Point p1 = *this - Other;
		float dis = std::hypotf((float)p1.x, (float)p1.y);
		return dis;
	}

	Point operator+(const Point& Other) const
	{
		return {x+Other.x, y+ Other.y};
	}
};
// GeoIndex is an index for GeoPoints within CoordinateSystem that provides fast range queries.
class GeoHash {
public:

	static uint64_t encode(const Point p1)
	{
		return encode(p1.x, p1.y);
	}
	static uint64_t encode(const int32_t _x, const int32_t _y)
	{
		uint64_t uid = 0;
		int x_bit; 
		int y_bit;
		//map int32 to uint32
		//-MaxInt ---> 0      ----> +MaxInt
		//	0	  ---> MaxInt ----> +2MaxInt 
		const int32_t minInt = std::numeric_limits<int32_t>::min();
		uint32_t x = (uint32_t)((int64_t)_x -minInt);
		uint32_t y = (uint32_t)((int64_t)_y -minInt);
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
		int bit=0;
		uint32_t x = 0; 
		uint32_t y = 0;
		const int32_t minInt = std::numeric_limits<int32_t>::min();

		for (int i = 0; i < 32; ++i)
 		{
			bit =(uid >>  ((31 - i) * 2)) & 0x3;
			x <<= 1;
			y <<= 1;
			x |= (bit >> 1);
			y |= (bit & 0x1);
		}
		int32_t _x = (int32_t)((int64_t)x + minInt);
		int32_t _y = (int32_t)((int64_t)y + minInt);
		Point p1(_x, _y);
		return p1;
	}

	static float distance(const uint64_t p1, const uint64_t p2)
	{
		Point _p1 = decode(p1);
		Point _p2 = decode(p2);
		Point _p = _p1 - _p2;
		float dis = std::hypot((float)_p.x, (float)_p.y);
		return dis;
	}
};

class GeoHashManager {
public:
	bool Add(int32_t x, int32_t y);
	bool Add(Point& point)
	{
		uint64_t hash = GeoHash::encode(point);
		auto ret = _data.insert({ hash, &point });
		return ret.second;
	}
	bool Remove(Point point)
	{
		uint64_t hash = GeoHash::encode(point);
		auto ret = _data.erase(hash);
		return ret > 0;
	}

	Point* SearchNearestPointInRange(Point point, const Point* p1, float currentDis, float stopDistance = 2)
	{
		Point* nearstPoint = nullptr;
		Point* foundPoint = nullptr; 
		//如果最近的点在范围外面，尝试范围查找
		Point dir1 = *p1 - point;
		// 以point为矩形的中心，p1为一个顶点，求出其他三个顶点 p2,p3,p4
		{
			//计算p2
			if(dir1.y != 0)
			{
				Point dir2(dir1.x, -dir1.y);
				Point p2 = point + dir2;
				if(FindInRange(p2, point, currentDis, nearstPoint))
				{
					foundPoint = nearstPoint;
				}
			}
		}
		{
			//计算p3
			if(dir1.x != 0)
			{
				Point dir3(-dir1.x, dir1.y);
				Point p3 = point + dir3;
				if(FindInRange(p3, point, currentDis, nearstPoint))
				{
					foundPoint = nearstPoint;
				}
			}
		}
		{
			//计算p4
			if(dir1.y != 0 && dir1.x != 0)
			{
				Point dir4(-dir1.x, -dir1.y);
				Point p4 = point + dir4;
				if(FindInRange(p4, point, currentDis, nearstPoint))
				{
					foundPoint = nearstPoint;
				}
			}
		}
		//递归查找更小的区域
		if(foundPoint != nullptr && currentDis > stopDistance)
		{
			if(Point* ret = SearchNearestPointInRange(point, foundPoint,currentDis))
			{
				//如果找到更新foundPoint
				foundPoint = ret;
			}
		}
	
		return foundPoint;
	}

	const Point* FindOne(Point point, float range = 2)
	{
// #ifdef PROFILING
// #endif
		ScopeProfiling sc("[FindOne]");
		uint64_t hash = GeoHash::encode(point);
		auto list = _data.equal_range(hash);
		//log(&list);
		uint64_t ret = 0;
		auto firtstBound = list.first;
		auto secondBound = list.second;
		const Point* p1 = firtstBound->second;
		float currentDis = point.Dist(*p1);
		if (currentDis > range)
		{
			if(const Point* tmp = SearchNearestPointInRange(point, p1, currentDis, range))
			{
				p1 = tmp;
			}
		}
		
		auto endTime = std::chrono::system_clock::now();
		return p1;
	}

	/**
	 * \brief 寻找检测地点 centerPoint，距离targetPoint小于currentCost最近的点
	 * \param checkPoint 检测地点
	 * \param targetPoint 
	 * \param currentCost 
	 * \param resultList 
	 * \return 是否查找成功
	 */
	bool FindInRange(const Point& checkPoint, const Point& targetPoint, float& currentCost, Point*& resultList)
	{
		uint64_t hash = GeoHash::encode(checkPoint);
		auto list = _data.equal_range(hash);
		auto firtstBound = list.first;
		auto secondBound = list.second;
		bool found = false;
		if (firtstBound != _data.end())
		{
			Point* p1 = firtstBound->second;
			float dis = targetPoint.Dist(*p1);
			if (dis < currentCost)
			{
				currentCost = dis;
				resultList = p1;
				found = true;
			}
		}
		return found;
	}
	
	void log(bool detail = false)
	{
		std::cout << "Size:" <<_data.size() << "[";
		if(detail)
		{
			for (auto it : _data)
			{
				std::cout << it.first <<"=>";
				it.second->Log();
			}
		}
		std::cout << "]" << std::endl;
		
	}

	void clear()
	{
		_data.clear();
	}


private:
	//bool isNear
private:
	std::map<uint64_t, Point*> _data;
public:
	static GeoHashManager& Instance() {
		if (!_instance)
		{
			_instance = new GeoHashManager();
		}
		return *_instance;
	}
private:
	static GeoHashManager* _instance;
};
GeoHashManager* GeoHashManager::_instance = nullptr;

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

#include "test.h"

#include <algorithm>
#include <bitset>
#include <random>
#include <set>

#include "geohash.h"

TEST(GeoIndexBasic) {
	{
		Point p1(1, 1);
		uint64_t uid = GeoHash::encode(p1);
		Point p2 = GeoHash::decode(uid);
		ASSERT_TRUE(p2.x == p1.x, "p1.x equal p2.x");
		ASSERT_TRUE(p2.y == p1.y);
	}
	{
		Point p1;
		p1.x = std::numeric_limits<int32_t>::max();
		p1.y = std::numeric_limits<int32_t>::max();
		uint64_t uid2 = GeoHash::encode(p1);
		Point p2 = GeoHash::decode(uid2);
		ASSERT_TRUE(p2.x == p1.x, "p1.x equal p2.x");
		ASSERT_TRUE(p2.y == p1.y);

	}
	{
		Point p1;
		p1.x = std::numeric_limits<int32_t>::min();
		p1.y = std::numeric_limits<int32_t>::min();
		uint64_t uid2 = GeoHash::encode(p1);
		Point p2 = GeoHash::decode(uid2);
		ASSERT_TRUE(p2.x == p1.x, "p1.x equal p2.x");
		ASSERT_TRUE(p2.y == p1.y);

	}

  //E.seed(time(NULL));
  //Cleaner cleaner([&](){remove(TEST_FILENAME);});
  //std::vector<IndexRecord> points;
  //for (int i = 0; i < 1000000; ++i) {
  //  points.push_back(IndexRecord(AnchoredGeoPoint(X(E), Y(E))));
  //}
  //AnchoredGeoIndex::order(points.begin(), points.end(), &IndexRecordSerializer::geopoint, points);
  //Buffer buffer(Buffer::LITTLE, points.size() * IndexRecordSerializer::ENCODED_SIZE);
  //for (const auto& next : points) {
  //  IndexRecordSerializer::write(next, buffer);
  //}   
  //buffer.switch_mode();
  //FILE* file = fopen(TEST_FILENAME, "wb");
  //fwrite(buffer.data(), 1, buffer.remaining(), file);
  //fclose(file);
  //AnchoredGeoIndex::FileDataSource file_source(
  //    TEST_FILENAME, &IndexRecordSerializer::parse, IndexRecordSerializer::ENCODED_SIZE,
  //    Buffer::LITTLE, 100);
  //AnchoredGeoIndex::VectorDataSource vector_source(points);
  //for (int i = 0; i < 100; ++i) {
  //  AnchoredGeoIndex file_index(&file_source, &IndexRecordSerializer::geopoint);
  //  AnchoredGeoIndex vector_index(&vector_source, &IndexRecordSerializer::geopoint);
  //  AnchoredGeoRectangle rect = MakeRandomGeoRectangle(0.01, 2);

  //  std::unordered_set<AnchoredGeoPoint> expected;
  //  for (auto& next : points) {
  //    if (rect.contains(next.point)) {
  //      expected.insert(next.point);
  //    }
  //  }

  //  std::unordered_set<AnchoredGeoPoint> file_actual;
  //  std::unordered_set<AnchoredGeoPoint> vector_actual;
  //  file_index.query(rect, [&](const IndexRecord& record){file_actual.insert(record.point);});
  //  vector_index.query(rect, [&](const IndexRecord& record){vector_actual.insert(record.point);});
  //  ASSERT_TRUE(expected == file_actual);
  //  ASSERT_TRUE(expected == vector_actual);
  //}
}
TEST(GeoDistanceSearchCaseSmall) {
	std::vector<Point> points{ {0, 8},{8, 1},{2, -4},{2, -1} ,{5, -8},{7, -4}, {-8, 8}, {-8, -5}, {-7, -6}, {-6, -3} };
	for (auto& it : points)
	{
		GeoHashManager::Instance().Add(it);
	}
	//Size:10[64=>(0, 8) 129=>(8, 1) 6148914691236517208=>(2, -4) 6148914691236517213=>(2, -1) 6148914691236517218=>(5, -8) 6148914691236517242=>(7, -4) 12297829382473034432=>(-8, 8) 18446744073709551557=>(-8, -5) 18446744073709551558=>(-7, -6) 18446744073709551577=>(-6, -3) ]
	GeoHashManager::Instance().log();
	Point point(6, -6);
	const Point* p1 = GeoHashManager::Instance().FindOne(point);
	point.reset(-6, 6);
	p1 = GeoHashManager::Instance().FindOne(point);
    ASSERT_TRUE(Point(*p1) == points[6]);

	point.reset(-2, 3);
	p1 = GeoHashManager::Instance().FindOne(point);
	p1->Log();
    //ASSERT_TRUE(p1 == points[3]);
	point.reset(4, -4);
	p1 = GeoHashManager::Instance().FindOne(point);
	p1->Log();
	
}
TEST(GeoZCreate) {
	return;
	//distribution.reset()
  //E.seed(time(NULL));
  //Cleaner cleaner([&](){remove(TEST_FILENAME);});
	GeoHashManager::Instance().clear();

	std::vector<Point*> points;
	for (int i = 0; i < 5; ++i)
	{
		for (int j=0;j<5;++j)
		{
			Point* point = new Point(i, j);
			points.push_back(point);
			GeoHashManager::Instance().Add(*point);

		}
		//points.push_back(IndexRecord(AnchoredGeoPoint(X(E), Y(E))));
	}
	GeoHashManager::Instance().log();
}
TEST(GeoDistanceSearchCase) {
	// return;
	std::default_random_engine generator;
	std::uniform_int_distribution<int32_t> distribution(-3390933, 3390933);
	auto GenRndNum = std::bind(distribution, generator);
	//distribution.reset()
  //E.seed(time(NULL));
  //Cleaner cleaner([&](){remove(TEST_FILENAME);});
	std::vector<Point*> points;
	for (int i = 0; i < 200000; ++i)
	{
		Point* point = new Point(GenRndNum(), GenRndNum());
		points.push_back(point);
		GeoHashManager::Instance().Add(*point);

		//points.push_back(IndexRecord(AnchoredGeoPoint(X(E), Y(E))));
	}
	Point point(6, -6);
	const Point* p1 = GeoHashManager::Instance().FindOne(point);
	p1->Log();
	point.reset(335909,-168609);
	p1 = GeoHashManager::Instance().FindOne(point);
	p1->Log();
	GeoHashManager::Instance().log();

	//GeoHashManager::Instance().log();
	//AnchoredGeoIndex::order(points.begin(), points.end(), &IndexRecordSerializer::geopoint, points);
	//Buffer buffer(Buffer::LITTLE, points.size() * IndexRecordSerializer::ENCODED_SIZE);
	//for (const auto& next : points) {
	//  IndexRecordSerializer::write(next, buffer);
	//}   
	//buffer.switch_mode();
	//FILE* file = fopen(TEST_FILENAME, "wb");
	//fwrite(buffer.data(), 1, buffer.remaining(), file);
	//fclose(file);
	//AnchoredGeoIndex::FileDataSource source(
	//    TEST_FILENAME, &IndexRecordSerializer::parse, IndexRecordSerializer::ENCODED_SIZE,
	//    Buffer::LITTLE, 100);
	//AnchoredGeoIndex index(&source, &IndexRecordSerializer::geopoint);
	//AnchoredGeoRectangle rect(-1, -1, 1, 1);

	//std::unordered_set<AnchoredGeoPoint> expected;
	//for (auto& next : points) {
	//  if (rect.contains(next.point)) {
	//    expected.insert(next.point);
	//  }
	//}

	//std::unordered_set<AnchoredGeoPoint> actual;
	//index.query(rect, [&](const IndexRecord& record){actual.insert(record.point);});
	//ASSERT_TRUE(expected == actual
}

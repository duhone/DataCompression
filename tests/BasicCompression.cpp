#include "catch.hpp"
#include "LosslessCompression.h"
#include <Platform\MemoryMappedFile.h>
#include <Platform\PathUtils.h>
#include <core\Timer.h>

using namespace std;
using namespace CR::DataCompression;
using namespace CR::Platform;

TEST_CASE("small txt compression", "lossless")
{
	auto testFilePath = RelativeToAbsolute("../../DataCompression/tests/data/pg571.txt");
	auto testFileMMap = OpenMMapFile(testFilePath.c_str());

	vector<uint8_t> compressedData;
	vector<uint8_t> decompressedData;

	auto test = [&](const char* label, LosslessCoompressionLevel level)
	{
		printf("%s test\n", label);
		{
			CR::Core::ScopedTimer time("compress time");
			compressedData = Compress(testFileMMap->data(), (uint32_t)testFileMMap->size(), level);
		}
		{
			CR::Core::ScopedTimer time("decompress time");
			decompressedData = Decompress(compressedData.data(), (uint32_t)compressedData.size());
		}
		REQUIRE(decompressedData.size() == testFileMMap->size());
		REQUIRE(memcmp(data(decompressedData), testFileMMap->data(), size(compressedData)) == 0);
		printf("compression ration %0.2f\n\n", ((float)decompressedData.size() / compressedData.size()));
	};

	test("txt lz4 fast", LosslessCoompressionLevel::Fast);;
	test("txt lz4 mixed", LosslessCoompressionLevel::Mixed);
	test("txt zlib", LosslessCoompressionLevel::Medium);
	test("txt lzma", LosslessCoompressionLevel::High);
}

TEST_CASE("simple image compression", "lossless")
{
	auto testFilePath = RelativeToAbsolute("../../DataCompression/tests/data/artificial.tif");
	auto testFileMMap = OpenMMapFile(testFilePath.c_str());

	vector<uint8_t> compressedData;
	vector<uint8_t> decompressedData;

	auto test = [&](const char* label, LosslessCoompressionLevel level)
	{
		printf("%s test\n", label);
		{
			CR::Core::ScopedTimer time("compress time");
			compressedData = Compress(testFileMMap->data(), (uint32_t)testFileMMap->size(), level);
		}
		{
			CR::Core::ScopedTimer time("decompress time");
			decompressedData = Decompress(compressedData.data(), (uint32_t)compressedData.size());
		}
		REQUIRE(decompressedData.size() == testFileMMap->size());
		REQUIRE(memcmp(data(decompressedData), testFileMMap->data(), size(compressedData)) == 0);
		printf("compression ration %0.2f\n\n", ((float)decompressedData.size() / compressedData.size()));
	};

	test("photo lz4 fast", LosslessCoompressionLevel::Fast);;
	test("photo lz4 mixed", LosslessCoompressionLevel::Mixed);
	test("photo zlib", LosslessCoompressionLevel::Medium);
	test("photo lzma", LosslessCoompressionLevel::High);
}

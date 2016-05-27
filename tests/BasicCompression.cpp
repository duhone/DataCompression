#include "catch.hpp"
#include "LosslessCompression.h"
#include <Platform\MemoryMappedFile.h>
#include <Platform\PathUtils.h>
#include <core\Timer.h>

using namespace std;
using namespace CR::DataCompression;
using namespace CR::Platform;

TEST_CASE("basic compression", "lossless")
{
	auto test = [](const char * testFile) {
		printf("%s test\n", testFile);
		auto testFilePath = RelativeToAbsolute(testFile);
		auto testFileMMap = OpenMMapFile(testFilePath.c_str());

		vector<uint8_t> sourceData(testFileMMap->data(), testFileMMap->data()+ testFileMMap->size());
		vector<uint8_t> compressedData;
		vector<uint8_t> decompressedData;

		auto test1 = [&](const char* label, LosslessCoompressionLevel level) {
			printf("%s mode\n", label);
			{
				CR::Core::ScopedTimer time("compress time");
				compressedData = Compress(sourceData.data(), (uint32_t)sourceData.size(), level);
			}
			{
				CR::Core::ScopedTimer time("decompress time");
				decompressedData = Decompress(compressedData.data(), (uint32_t)compressedData.size());
			}
			REQUIRE(decompressedData.size() == testFileMMap->size());
			REQUIRE(memcmp(data(decompressedData), testFileMMap->data(), size(compressedData)) == 0);
			printf("compression ration %0.2f\n\n", ((float)decompressedData.size() / compressedData.size()));
		};

		test1("txt lz4 fast", LosslessCoompressionLevel::Fast);;
		test1("txt lz4 mixed", LosslessCoompressionLevel::Mixed);
		test1("txt zlib", LosslessCoompressionLevel::Medium);
		test1("txt lzma", LosslessCoompressionLevel::High);
	};

	const char* testFiles[] = {
		"../../DataCompression/tests/data/alice29.txt",
		"../../DataCompression/tests/data/cp.html",
		"../../DataCompression/tests/data/data.xml",
		"../../DataCompression/tests/data/game.cpp",
		"../../DataCompression/tests/data/TitleThemeRemix.wav",
		"../../DataCompression/tests/data/bumble_tales.tga",
		"../../DataCompression/tests/data/kodim23.tga"
	};

	for(const auto & testFile : testFiles)
	{
		test(testFile);
	}
}

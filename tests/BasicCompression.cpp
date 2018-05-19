#include "catch.hpp"
#include "DataCompression\LosslessCompression.h"
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

		auto test1 = [&](const char* label, LosslessCompressionLevel level) {
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

		test1("fast: lz4", LosslessCompressionLevel::Fast);;
		test1("mixed: lz4 hc", LosslessCompressionLevel::Mixed);
		test1("medium: zstd 3", LosslessCompressionLevel::Medium);
		test1("high: zstd 18", LosslessCompressionLevel::High);
		test1("archive: lzma", LosslessCompressionLevel::Archive);
		test1("zlib", LosslessCompressionLevel::ZLIB);
	};

	const char* testFiles[] = {
		"./alice29.txt",
		"./cp.html",
		"./data.xml",
		"./game.cpp",
		"./TitleThemeRemix.wav",
		"./bumble_tales.tga",
		"./kodim23.tga"
	};

	for(const auto & testFile : testFiles)
	{
		test(testFile);
	}
}

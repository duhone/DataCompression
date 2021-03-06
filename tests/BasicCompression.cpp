﻿#include <3rdParty/doctest.h>

#include "DataCompression\LosslessCompression.h"
#include "Platform\MemoryMappedFile.h"
#include "Platform\PathUtils.h"
#include "core\Timer.h"

using namespace std;
using namespace CR;
using namespace CR::DataCompression;
using namespace CR::Platform;

TEST_CASE("basic compression") {
	auto test = [](const char* testFile) {
		printf("%s test\n", testFile);
		auto testFilePath = GetCurrentProcessPath() / testFile;
		MemoryMappedFile testFileMMap(testFilePath.c_str());

		Core::storage_buffer<byte> compressedData;
		Core::storage_buffer<byte> decompressedData;

		auto test1 = [&](const char* label, int32_t level) {
			printf("%s mode\n", label);
			{
				CR::Core::ScopedTimer time("compress time");
				compressedData =
				    Compress(Core::Span<const std::byte>(testFileMMap.data(), (uint32_t)testFileMMap.size()), level);
			}
			{
				CR::Core::ScopedTimer time("decompress time");
				decompressedData =
				    Decompress(Core::Span<const std::byte>(compressedData.data(), (uint32_t)compressedData.size()));
			}
			REQUIRE(decompressedData.size() == testFileMMap.size());
			REQUIRE(memcmp(data(decompressedData), testFileMMap.data(), size(compressedData)) == 0);
			printf("compression ration %0.2f\n\n", ((float)decompressedData.size() / compressedData.size()));
		};

		test1("level -5", -5);
		test1("level -1", -1);
		test1("level 1", 1);
		test1("level 3", 3);
		test1("level 6", 6);
		test1("level 9", 9);
		test1("level 12", 12);
		test1("level 15", 15);
		test1("level 18", 18);
		test1("level 22", 22);
	};

	const char* testFiles[] = {"./alice29.txt",         "./cp.html",          "./data.xml",   "./game.cpp",
	                           "./TitleThemeRemix.wav", "./bumble_tales.tga", "./kodim23.tga"};

	for(const auto& testFile : testFiles) { test(testFile); }
}

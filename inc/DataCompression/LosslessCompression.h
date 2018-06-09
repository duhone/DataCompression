#pragma once
#include <vector>

namespace CR
{
	namespace DataCompression
	{
    // a_level is -5 to 22. 3 is a good default if you need fairly fast compression and decompression.
    // 18 is best if you need fast decompression, but dont care about compression speed. -5 if you need fastest possible.
		std::vector<uint8_t> Compress(const void* a_src, uint32_t a_srcSize, int32_t a_level);
		std::vector<uint8_t> Decompress(const void* a_src, uint32_t a_srcSize);
	}
}
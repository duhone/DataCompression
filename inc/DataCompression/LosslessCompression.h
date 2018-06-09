#pragma once
#include <vector>

namespace CR
{
	namespace DataCompression
	{
		std::vector<uint8_t> Compress(const void* a_src, uint32_t a_srcSize, uint32_t a_level);
		std::vector<uint8_t> Decompress(const void* a_src, uint32_t a_srcSize);
	}
}
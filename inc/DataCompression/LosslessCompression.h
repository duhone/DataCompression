#pragma once
#include <vector>

namespace CR
{
	namespace DataCompression
	{
		enum class LosslessCoompressionLevel : uint32_t
		{
			Fast, //lz4
			Mixed, //hclz4
			Medium, //zlib
			High //lzma
		};

		std::vector<uint8_t> Compress(const void* a_src, uint32_t a_srcSize, LosslessCoompressionLevel a_level);
		std::vector<uint8_t> Decompress(const void* a_src, uint32_t a_srcSize);
	}
}
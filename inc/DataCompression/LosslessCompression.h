#pragma once
#include <vector>

namespace CR
{
	namespace DataCompression
	{
		//If you need both fast compression and decompression use Fast or Medium, if compression is offline
		//and you only need decompression to be fast, use mixed or high. Archive is too slow for anything most of the time.
		enum class LosslessCompressionLevel : uint32_t
		{
			//lz4, very fast compression and decompression, much faster than zlib
			Fast, 
			//hclz4 slow compression, very fast decompression, usefull for offline compression
			//compression is an order of magnitude slower than Fast, decompression speed about the same
			//as fast, improved compression ratio
			Mixed, 
			//zstd 3 balanced mode like fast but slower.
			//about same compression ratio as zlib, compression speed is fast, much faster than mixed, but 
			//a few x slower than fast. decompression speed is a few x slower than fast and mixed as well, but much
			//faster than zlib
			Medium,
			//zstd 18 slow compression, fast decompression. much higher compression ratio as zlib,
			High, 
			//lzma super slow on both compression and decompression, highest compresion
			Archive,
			//not intended to be used, for reference/comparison
			ZLIB
		};

		std::vector<uint8_t> Compress(const void* a_src, uint32_t a_srcSize, LosslessCompressionLevel a_level);
		std::vector<uint8_t> Decompress(const void* a_src, uint32_t a_srcSize);
	}
}
#include "LosslessCompression.h"
#include <lz4.h>
#include <lz4hc.h>
#include <zlib.h>
#include <LzmaLib.h>
#include <zstd.h>
#include <exception>
#include "core\literals.h"

using namespace CR::DataCompression;
using namespace CR::Core::Literals;

namespace
{
	const int c_CompFourCC{ 'CRCF' };
	const int c_CompVersion{ 1 };

	#pragma pack(4)
	struct CompressionHeader
	{
		int FourCC{ c_CompFourCC };
		uint32_t Version{ c_CompVersion };
		LosslessCompressionLevel CompLevel{ LosslessCompressionLevel::Fast };
		uint32_t DecompressedSize;
		uint32_t CompressedSize;
	}; 
	#pragma pack()
}
std::vector<uint8_t> CR::DataCompression::Compress(const void* a_src, uint32_t a_srcSize,
	LosslessCompressionLevel a_level)
{
	a_src; a_srcSize; a_level;
	std::vector<uint8_t> result;
	
	int bound = 0;
	switch (a_level)
	{
	case LosslessCompressionLevel::Fast:
		bound = LZ4_compressBound((int)a_srcSize);
		break;
	case LosslessCompressionLevel::Mixed:
		bound = LZ4_compressBound((int)a_srcSize);
		break;
	case LosslessCompressionLevel::Medium:
		bound = compressBound(a_srcSize);
		break;
	case LosslessCompressionLevel::High:
		bound = a_srcSize + a_srcSize / 2 + LZMA_PROPS_SIZE; //pulled 1.5 from some old code of mine, no clue where it comes from
		break;
	case LosslessCompressionLevel::Test:
		bound = (int)ZSTD_compressBound(a_srcSize);
		break;
	};
	result.resize(bound+sizeof(CompressionHeader));
	auto header = new(std::data(result)) CompressionHeader;
	header->CompLevel = a_level;
	header->DecompressedSize = a_srcSize;
	switch (a_level)
	{
	case LosslessCompressionLevel::Fast:
		header->CompressedSize = LZ4_compress_default((const char*)a_src, (char*)result.data() + sizeof(CompressionHeader),
			(int)a_srcSize, bound);
		break;
	case LosslessCompressionLevel::Mixed:
		header->CompressedSize = LZ4_compress_HC((const char*)a_src, (char*)result.data() + sizeof(CompressionHeader),
			(int)a_srcSize, bound, 9);
		break;
	case LosslessCompressionLevel::Medium:
		header->CompressedSize = bound;
		compress2((Bytef*)result.data() + sizeof(CompressionHeader), (uLongf*)&header->CompressedSize,
			(const Bytef*)a_src, (int)a_srcSize, 7);
		break;
	case LosslessCompressionLevel::High:
	{
		size_t propSize = LZMA_PROPS_SIZE;
		size_t compressedSize = bound - LZMA_PROPS_SIZE;
		LzmaCompress(data(result) + LZMA_PROPS_SIZE + sizeof(CompressionHeader), &compressedSize, (const unsigned char*)a_src, a_srcSize,
			data(result) + sizeof(CompressionHeader), &propSize, 5, (unsigned int)256_Kb, -1, -1, -1, -1, -1);
		header->CompressedSize = (uint32_t)compressedSize + LZMA_PROPS_SIZE;
	}
		break;
	case LosslessCompressionLevel::Test:
		header->CompressedSize = (uint32_t)ZSTD_compress(result.data() + sizeof(CompressionHeader), bound,
			a_src, a_srcSize, 0);
		break;
	};
	result.resize(header->CompressedSize + sizeof(CompressionHeader));

	return result;
}

std::vector<uint8_t> CR::DataCompression::Decompress(const void* a_src, uint32_t a_srcSize)
{
	std::vector<uint8_t> result;
	if (a_srcSize < sizeof(CompressionHeader))
		return result;

	auto * header = reinterpret_cast<const CompressionHeader*>(a_src);
	if (header->FourCC != c_CompFourCC)
		throw std::exception("compressed data has invalid fourcc");
	if (header->Version != c_CompVersion)
		throw std::exception("compressed data has invalid version");
	if (header->CompressedSize > (a_srcSize - sizeof(CompressionHeader)))
		throw std::exception("compressed data size does not match header");

	result.resize(header->DecompressedSize);
	int bytesRead = 0;
	switch (header->CompLevel)
	{
	case LosslessCompressionLevel::Fast:
		bytesRead = LZ4_decompress_fast((const char *)a_src + sizeof(CompressionHeader), (char*)data(result),
			header->DecompressedSize);
		break;
	case LosslessCompressionLevel::Mixed:
		bytesRead = LZ4_decompress_fast((const char *)a_src + sizeof(CompressionHeader), (char*)data(result),
			header->DecompressedSize);
		break;
	case LosslessCompressionLevel::Medium:
		bytesRead = header->DecompressedSize;
		uncompress((Bytef*)data(result), (uLongf*)&bytesRead,
			(const Bytef*)a_src + sizeof(CompressionHeader), header->CompressedSize);
		if (bytesRead == (int)header->DecompressedSize)
			bytesRead = header->CompressedSize;
		break;
	case LosslessCompressionLevel::High:
	{
		size_t decompSize = header->DecompressedSize;
		size_t srcSize = a_srcSize - LZMA_PROPS_SIZE;
		LzmaUncompress(data(result), &decompSize, (unsigned char*)a_src + sizeof(CompressionHeader) + LZMA_PROPS_SIZE,
			&srcSize, (unsigned char*)a_src + sizeof(CompressionHeader), LZMA_PROPS_SIZE);
		bytesRead = (int)srcSize + LZMA_PROPS_SIZE;
	}
		break;
	case LosslessCompressionLevel::Test:
		bytesRead = (int)ZSTD_decompress(data(result), header->DecompressedSize, (char*)a_src + sizeof(CompressionHeader), header->CompressedSize);
		if (bytesRead == (int)header->DecompressedSize)
			bytesRead = header->CompressedSize;
		break;
	};
	if(bytesRead != (int)header->CompressedSize)
		throw std::exception("did not decompress the expected amount of data");

	return result;
}
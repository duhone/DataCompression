#include "DataCompression/LosslessCompression.h"
#include "core\literals.h"
#include <exception>
#include <zstd.h>

using namespace CR::DataCompression;
using namespace CR::Core::Literals;

namespace {
	const int c_CompFourCC{'CRCF'};
	const int c_CompVersion{2};

#pragma pack(4)
	struct CompressionHeader {
		int FourCC{c_CompFourCC};
		uint32_t Version{c_CompVersion};
		uint32_t DecompressedSize;
		uint32_t CompressedSize;
	};
#pragma pack()
}    // namespace

std::vector<uint8_t> CR::DataCompression::Compress(const void* a_src, uint32_t a_srcSize, int32_t a_level) {
	std::vector<uint8_t> result;

	int bound = (int)ZSTD_compressBound(a_srcSize);

	result.resize(bound + sizeof(CompressionHeader));
	auto header              = new(std::data(result)) CompressionHeader;
	header->DecompressedSize = a_srcSize;

	header->CompressedSize =
	    (uint32_t)ZSTD_compress(result.data() + sizeof(CompressionHeader), bound, a_src, a_srcSize, a_level);

	result.resize(header->CompressedSize + sizeof(CompressionHeader));

	return result;
}

std::vector<uint8_t> CR::DataCompression::Decompress(const void* a_src, uint32_t a_srcSize) {
	std::vector<uint8_t> result;
	if(a_srcSize < sizeof(CompressionHeader)) return result;

	auto* header = reinterpret_cast<const CompressionHeader*>(a_src);
	if(header->FourCC != c_CompFourCC) throw std::exception("compressed data has invalid fourcc");
	if(header->Version != c_CompVersion) throw std::exception("compressed data has invalid version");
	if(header->CompressedSize > (a_srcSize - sizeof(CompressionHeader)))
		throw std::exception("compressed data size does not match header");

	result.resize(header->DecompressedSize);
	int bytesRead = (int)ZSTD_decompress(data(result), header->DecompressedSize,
	                                     (char*)a_src + sizeof(CompressionHeader), header->CompressedSize);

	if(bytesRead != (int)header->DecompressedSize)
		throw std::exception("did not decompress the expected amount of data");

	return result;
}

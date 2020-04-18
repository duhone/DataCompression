#include "DataCompression/LosslessCompression.h"

#include "core/Log.h"
#include "core/literals.h"

#include <exception>
#include <zstd.h>

using namespace CR;
using namespace CR::DataCompression;
using namespace CR::Core::Literals;

namespace {
	const int c_CompFourCC{'CRCF'};
	const int c_CompVersion{2};

#pragma pack(4)
	struct CompressionHeader {
		int FourCC{c_CompFourCC};
		uint32_t Version{c_CompVersion};
		uint32_t DecompressedSize{0};
		uint32_t CompressedSize{0};
	};
#pragma pack()
}    // namespace

Core::storage_buffer<std::byte> CR::DataCompression::Compress(Core::Span<std::byte> a_src, int32_t a_level) {
	Core::storage_buffer<std::byte> result;

	int bound = (int)ZSTD_compressBound(a_src.size());

	result.prepare(bound + sizeof(CompressionHeader));
	auto header              = new(std::data(result)) CompressionHeader;
	header->DecompressedSize = (uint32_t)a_src.size();

	header->CompressedSize =
	    (uint32_t)ZSTD_compress(result.data() + sizeof(CompressionHeader), bound, a_src.data(), a_src.size(), a_level);

	result.commit(header->CompressedSize + sizeof(CompressionHeader));

	return result;
}

Core::storage_buffer<std::byte> CR::DataCompression::Decompress(Core::Span<std::byte> a_src) {
	Core::storage_buffer<std::byte> result;
	if(a_src.size() < sizeof(CompressionHeader)) return result;

	auto* header = reinterpret_cast<const CompressionHeader*>(a_src.data());
	Core::Log::Require(header->FourCC == c_CompFourCC, "compressed data has invalid fourcc");
	Core::Log::Require(header->Version == c_CompVersion, "compressed data has invalid version");
	Core::Log::Require(header->CompressedSize <= a_src.size() - sizeof(CompressionHeader),
	                   "compressed data size does not match header");

	result.prepare(header->DecompressedSize);
	int bytesRead = (int)ZSTD_decompress(data(result), header->DecompressedSize,
	                                     (char*)a_src.data() + sizeof(CompressionHeader), header->CompressedSize);

	Core::Log::Require(bytesRead == (int)header->DecompressedSize, "did not decompress the expected amount of data");

	result.commit(header->DecompressedSize);

	return result;
}

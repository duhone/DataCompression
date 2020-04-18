#pragma once

#include "core/Span.h"
#include "core/storage_buffer.h"

#include <cstddef>

namespace CR::DataCompression {
	// a_level is -5 to 22. 3 is a good default if you need fairly fast compression and decompression.
	// 18 is best if you need fast decompression, but dont care about compression speed. -5 if you need fastest
	// possible.
	Core::storage_buffer<std::byte> Compress(const Core::Span<const std::byte> a_src, int32_t a_level);
	Core::storage_buffer<std::byte> Decompress(const Core::Span<const std::byte> a_src);
}    // namespace CR::DataCompression

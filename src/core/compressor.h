#pragma once

#include <format>
#include <ios>
#include <istream>
#include <ostream>
#include <stdexcept>

namespace core {

// Compressor error.
class CompressorError;

// Virtual data compressor.
class Compressor {
	public:
		virtual ~Compressor() = default;

		// Virtual method for compressing data from `input` to `output`.
		virtual void compress(std::istream& input, std::ostream& output) = 0;

	protected:
		// Tries to replace current exception mask with `failbit | badbit` exception
		// mask.
		//
		// Returns exception mask before replacement.
		template <class CharT, class Traits>
		static std::ios_base::iostate set_ios_fail_and_bad_exceptions_(
				std::basic_ios<CharT, Traits>& ios) {
			// Save original exceptions to restore them later.
			const auto original_exceptions = ios.exceptions();
			// Try to set failbit and badbit exception mask.
			try {
				ios.exceptions(std::ios_base::failbit | std::ios_base::badbit);
			} catch (const std::ios_base::failure& e) {
				throw CompressorError(std::format(
					"failed to set failbit|badbit to compressor's ios: {}", e.what()));
			}
			return original_exceptions;
		}
};

class CompressorError: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

}  // namespace core

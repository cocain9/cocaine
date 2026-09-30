#pragma once

// generate 'pseudo-random' xor key based on file, date and line.
#define GET_XOR_KEYUI8  ( ( CONST_HASH( __FILE__ __TIMESTAMP__ ) + __LINE__ ) % UINT8_MAX )
#define GET_XOR_KEYUI16 ( ( CONST_HASH( __FILE__ __TIMESTAMP__ ) + __LINE__ ) % UINT16_MAX )
#define GET_XOR_KEYUI32 ( ( CONST_HASH( __FILE__ __TIMESTAMP__ ) + __LINE__ ) % UINT32_MAX )

// 'xor' is a C++ keyword; use a valid namespace with the same encoding logic.
namespace cocaine_strings {
	// actual xor implementation.
	template< class t, const size_t len, const t key >
	class Gen {
	private:
		std::array< t, len > m_buffer;

	private:
		// encrypt single character.
		constexpr t enc(const t c) const noexcept {
			return c ^ key;
		}

		// decrypt single character.
		__forceinline t dec(const t c) const noexcept {
			return c ^ key;
		}

	public:
		// iterate each byte and decrypt.
		__forceinline auto data() noexcept {
			for (size_t i{ 0u }; i < len; ++i)
				m_buffer[i] = dec(m_buffer[i]);

			return m_buffer.data();
		}

		// ctor.
		template< size_t... seq >
		constexpr __forceinline Gen(const t(&s)[len], std::index_sequence< seq... >) noexcept : m_buffer{ enc(s[seq])... } {}
	};
}

template< class t, const size_t len >
constexpr __forceinline auto XorStr(const t(&s)[len]) {
	// Return the owner, never a pointer into a temporary destroyed on return.
	return cocaine_strings::Gen< t, len, static_cast<t>(GET_XOR_KEYUI8) >(s, std::make_index_sequence< len >());
}

#ifdef _DEBUG
#define XOR( s ) ( s )
#else
// Each literal has its own process-lifetime buffer. Decode exactly once, using
// thread-safe local static initialization, so stored pointers remain valid.
#define XOR( s ) ([]() { \
    static auto storage = XorStr( s ); \
    static auto* text = storage.data(); \
    return text; \
}())
#endif

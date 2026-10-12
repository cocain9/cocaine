#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace pattern {
    using patterns_t = std::vector<Address>;
    using pattern_byte_t = std::pair<uint8_t, bool>;

    namespace detail {
        inline int hex_digit( char value ) {
            if( value >= '0' && value <= '9' ) return value - '0';
            if( value >= 'a' && value <= 'f' ) return value - 'a' + 10;
            if( value >= 'A' && value <= 'F' ) return value - 'A' + 10;
            return -1;
        }

        // Each whitespace-separated token is a byte or a full-byte wildcard.
        // Reject the entire signature on bad input; never scan a shortened pattern.
        inline std::vector<pattern_byte_t> parse( const std::string& text ) {
            std::vector<pattern_byte_t> bytes;
            std::istringstream stream( text );
            std::string token;
            while( stream >> token ) {
                if( token == "?" || token == "??" ) {
                    bytes.emplace_back( 0, true );
                    continue;
                }
                if( token.size( ) != 2 ) return {};
                const int high = hex_digit( token[0] );
                const int low = hex_digit( token[1] );
                if( high < 0 || low < 0 ) return {};
                bytes.emplace_back( static_cast<uint8_t>( high * 16 + low ), false );
            }
            return bytes;
        }

        inline bool valid_range( Address start, size_t len ) {
            const auto base = start.as<uintptr_t>( );
            return base != 0 && len != 0
                && len <= std::numeric_limits<uintptr_t>::max( ) - base
                && len <= static_cast<size_t>( std::numeric_limits<ptrdiff_t>::max( ) );
        }

        inline Address find( Address start, size_t len, const std::vector<pattern_byte_t>& bytes ) {
            if( bytes.empty( ) || bytes.size( ) > len || !valid_range( start, len ) ) return {};
            const auto begin = start.as<const uint8_t*>( );
            const auto end = begin + len;
            const auto match = std::search( begin, end, bytes.begin( ), bytes.end( ),
                []( uint8_t value, const pattern_byte_t& byte ) {
                    return byte.second || value == byte.first;
                } );
            return match == end ? Address{} : Address( match );
        }
    }

    // Caller must supply a readable buffer of len bytes for the duration of the scan.
    // A missing match, invalid signature, or invalid numeric range returns null.
    inline Address find( Address start, size_t len, const std::string& pat ) {
        return detail::find( start, len, detail::parse( pat ) );
    }

    inline Address find( const PE::Module& module, const std::string& pat ) {
        return find( module.GetBase( ), module.GetImageSize( ), pat );
    }

    // Required addresses must have exactly one match, including overlapping candidates.
    inline Address find_unique( Address start, size_t len, const std::string& pat ) {
        const auto bytes = detail::parse( pat );
        const auto first = detail::find( start, len, bytes );
        if( !first.as<uintptr_t>( ) ) return {};
        const size_t consumed = first.as<uintptr_t>( ) - start.as<uintptr_t>( ) + 1;
        if( detail::find( first.add( size_t{ 1 } ), len - consumed, bytes ).as<uintptr_t>( ) ) return {};
        return first;
    }

    inline Address require( const PE::Module& module, const std::string& pat ) {
        const auto match = find_unique( module.GetBase( ), module.GetImageSize( ), pat );
        if( !match.as<uintptr_t>( ) )
            throw std::runtime_error( "Cocaine: needit signature missin, ill-formit, or unclear in "
                + module.GetName( ) + ": " + pat );
        return match;
    }

    inline patterns_t FindAll( Address start, size_t len, const std::string& pat ) {
        patterns_t matches;
        const auto bytes = detail::parse( pat );
        while( len != 0 ) {
            const auto result = detail::find( start, len, bytes );
            if( !result.as<uintptr_t>( ) ) break;
            matches.push_back( result );
            // Advance one byte so overlapping matches are preserved.
            const size_t consumed = result.as<uintptr_t>( ) - start.as<uintptr_t>( ) + 1;
            len -= consumed;
            start = result.add( size_t{ 1 } );
        }
        return matches;
    }

    inline patterns_t FindAll( const PE::Module& module, const std::string& pat ) {
        return FindAll( module.GetBase( ), module.GetImageSize( ), pat );
    }
}

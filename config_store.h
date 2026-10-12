#pragma once

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>
#include "base64.h"
#include "json.h"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace config_store {
    constexpr std::size_t max_file_size = 1024 * 1024;

    inline bool valid_filename( const std::string& name ) {
        return !name.empty( ) && name != "." && name != ".."
            && name.find_first_of( "/\\:<>\"|?*" ) == std::string::npos
            && name.back( ) != '.' && name.back( ) != ' '
            && std::none_of( name.begin( ), name.end( ), []( unsigned char c ) { return c < 32; } );
    }

    inline bool valid_base64( const std::string& data ) {
        if( data.empty( ) || data.size( ) % 4 != 0 )
            return false;
        auto end = data.size( );
        if( data[end - 1] == '=' ) --end;
        if( end > 0 && data[end - 1] == '=' ) --end;
        for( std::size_t i = 0; i < end; ++i )
            if( !crypto::is_base64( static_cast<unsigned char>( data[i] ) ) )
                return false;
        return true;
    }

    // Bounded binary reads prevent failed tellg() conversions and text-mode short reads.
    inline bool read( const std::filesystem::path& file, nlohmann::json& result ) {
        std::ifstream stream( file, std::ios::binary | std::ios::ate );
        if( !stream ) return false;
        const auto size = stream.tellg( );
        if( size <= 0 || size > static_cast<std::streamoff>( max_file_size ) ) return false;
        std::string data( static_cast<std::size_t>( size ), '\0' );
        stream.seekg( 0 );
        if( !stream.read( data.data( ), static_cast<std::streamsize>( data.size( ) ) )
            || !valid_base64( data ) ) return false;
        try {
            auto parsed = nlohmann::json::parse( crypto::base64_decode( data ) );
            if( !parsed.is_object( ) ) return false;
            result = std::move( parsed );
            return true;
        }
        catch( const nlohmann::json::exception& ) {
            return false;
        }
    }

    // Serialize first, then replace only after the complete temporary file is closed.
    // Calls are serialized by the application's UI thread.
    inline bool write( const std::filesystem::path& file, const nlohmann::json& value ) {
        std::string data;
        try { data = crypto::base64_encode( value.dump( ) ); }
        catch( const nlohmann::json::exception& ) { return false; }
        if( data.size( ) > max_file_size ) return false;
        auto temporary = file;
        temporary += ".tmp";
        std::ofstream stream( temporary, std::ios::binary | std::ios::trunc );
        if( !stream ) return false;
        stream.write( data.data( ), static_cast<std::streamsize>( data.size( ) ) );
        stream.close( );
        bool replaced = false;
        if( stream ) {
#ifdef _WIN32
            replaced = MoveFileExW( temporary.c_str( ), file.c_str( ),
                MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH ) != 0;
#else
            std::error_code error;
            std::filesystem::rename( temporary, file, error );
            replaced = !error;
#endif
        }
        if( !replaced ) {
            std::error_code ignored;
            std::filesystem::remove( temporary, ignored );
        }
        return replaced;
    }

    inline bool integer( const nlohmann::json& value, int minimum, int maximum ) {
        if( !value.is_number_integer( ) ) return false;
        if( value.is_number_unsigned( ) )
            return maximum >= 0 && value.get<std::uint64_t>( ) <= static_cast<std::uint64_t>( maximum )
                && ( minimum <= 0 || value.get<std::uint64_t>( ) >= static_cast<std::uint64_t>( minimum ) );
        const auto number = value.get<std::int64_t>( );
        return number >= minimum && number <= maximum;
    }
}

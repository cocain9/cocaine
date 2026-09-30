#include "includes.h"
#include "config_store.h"
#include <limits>

Config g_config{};

namespace {
    struct LoadingGuard {
        bool& flag;
        const bool previous;
        explicit LoadingGuard( bool& value ) : flag( value ), previous( value ) { flag = true; }
        ~LoadingGuard( ) { flag = previous; }
    };
}

void Config::init( ) {
    m_init = false;
    char documents[MAX_PATH]{};
    if( FAILED( g_winapi.SHGetFolderPathA( nullptr, CSIDL_PERSONAL, nullptr,
        SHGFP_TYPE_CURRENT, documents ) ) ) return;

    m_path = std::filesystem::path( documents ) / "Cocaine";
    m_legacy_path = std::filesystem::path( documents ) / g_cl.m_user;
    std::error_code error;
    std::filesystem::create_directories( m_path, error );
    m_init = !error && std::filesystem::is_directory( m_path, error ) && !error;
}

bool Config::read( const std::string& name, nlohmann::json& value ) const {
    if( !m_init || !config_store::valid_filename( name ) ) return false;
    const auto file = m_path / name;
    std::error_code error;
    const bool exists = std::filesystem::exists( file, error );
    if( error ) return false;
    if( exists ) return config_store::read( file, value );

    // Only fall back when the new file is absent; never mask a corrupt new file.
    auto legacy = m_legacy_path / name;
    legacy.replace_extension( ".sup" );
    return config_store::read( legacy, value );
}

bool Config::LoadHotkeys( ) {
    nlohmann::json value;
    if( !read( "keys.cfg", value ) ) return false;
    const std::array<Keybind*, 6> keys{ &g_menu.main.config.key1, &g_menu.main.config.key2,
        &g_menu.main.config.key3, &g_menu.main.config.key4, &g_menu.main.config.key5,
        &g_menu.main.config.key6 };
    for( const auto key : keys ) {
        const auto entry = value.find( key->m_file_id );
        if( entry != value.end( ) && !config_store::integer( *entry, -1, 255 ) ) return false;
    }
    // Key callbacks normally save immediately. Do not overwrite the file mid-load.
    LoadingGuard loading( m_loading );
    for( const auto key : keys ) {
        const auto entry = value.find( key->m_file_id );
        if( entry != value.end( ) ) key->set( entry->get<int>( ) );
    }
    return true;
}

bool Config::SaveHotkeys( ) {
    if( !m_init || m_loading ) return false;
    nlohmann::json value = nlohmann::json::object( );
    for( const auto key : { &g_menu.main.config.key1, &g_menu.main.config.key2,
        &g_menu.main.config.key3, &g_menu.main.config.key4, &g_menu.main.config.key5,
        &g_menu.main.config.key6 } )
        value[key->m_file_id] = key->get( );
    return config_store::write( m_path / "keys.cfg", value );
}

bool Config::load( const Form* form, const std::string& name ) {
    if( !form || form->m_tabs.empty( ) ) return false;
    nlohmann::json config;
    if( !read( name, config ) ) return false;

    // Validate every known value before changing live state or firing callbacks.
    std::vector<std::function<void( )>> updates;
    for( auto tab_entry = config.begin( ); tab_entry != config.end( ); ++tab_entry ) {
        Tab* tab = form->GetTabByName( tab_entry.key( ) );
        if( !tab ) continue;
        if( !tab_entry.value( ).is_object( ) ) return false;
        for( auto entry = tab_entry.value( ).begin( ); entry != tab_entry.value( ).end( ); ++entry ) {
            Element* element = tab->GetElementByName( entry.key( ) );
            if( !element || !( element->m_flags & ElementFlags::SAVE ) ) continue;
            const auto& value = entry.value( );
            switch( element->m_type ) {
            case ElementTypes::CHECKBOX: {
                if( !value.is_boolean( ) ) return false;
                const bool enabled = value.get<bool>( );
                updates.emplace_back( [element, enabled] { static_cast<Checkbox*>( element )->set( enabled ); } );
                break;
            }
            case ElementTypes::SLIDER: {
                if( !value.is_number( ) ) return false;
                const double number = value.get<double>( );
                if( !std::isfinite( number ) || std::abs( number ) > std::numeric_limits<float>::max( ) ) return false;
                updates.emplace_back( [element, number] { static_cast<Slider*>( element )->set( static_cast<float>( number ) ); } );
                break;
            }
            case ElementTypes::KEYBIND:
            case ElementTypes::DROPDOWN:
            case ElementTypes::EDIT: {
                const bool dropdown = element->m_type == ElementTypes::DROPDOWN;
                const int maximum = element->m_type == ElementTypes::KEYBIND ? 255 : std::numeric_limits<int>::max( );
                if( !config_store::integer( value, dropdown ? 0 : -1, maximum ) ) return false;
                const int number = value.get<int>( );
                updates.emplace_back( [element, number] {
                    if( element->m_type == ElementTypes::KEYBIND ) static_cast<Keybind*>( element )->set( number );
                    else if( element->m_type == ElementTypes::DROPDOWN ) static_cast<Dropdown*>( element )->set( number );
                    else static_cast<Edit*>( element )->set( number );
                } );
                break;
            }
            case ElementTypes::COLORPICKER: {
                if( !value.is_array( ) || value.size( ) != 4 ) return false;
                for( const auto& channel : value )
                    if( !config_store::integer( channel, 0, 255 ) ) return false;
                const Color color{ value[0].get<int>( ), value[1].get<int>( ), value[2].get<int>( ), value[3].get<int>( ) };
                updates.emplace_back( [element, color] { static_cast<Colorpicker*>( element )->set( color ); } );
                break;
            }
            case ElementTypes::MULTI_DROPDOWN: {
                if( !value.is_array( ) ) return false;
                for( const auto& index : value )
                    if( !config_store::integer( index, 0, std::numeric_limits<int>::max( ) ) ) return false;
                const auto selection = value.get<std::vector<int>>( );
                updates.emplace_back( [element, selection] {
                    auto dropdown = static_cast<MultiDropdown*>( element );
                    dropdown->clear( );
                    for( const auto index : selection ) dropdown->select( index );
                } );
                break;
            }
            default: break;
            }
        }
    }
    LoadingGuard loading( m_loading );
    for( const auto& update : updates ) update( );
    return true;
}

bool Config::save( const Form* form, const std::string& name ) {
    if( !m_init || !form || form->m_tabs.empty( ) || !config_store::valid_filename( name ) ) return false;
    nlohmann::json config = nlohmann::json::object( );

	// iterate tabs.
	for( const auto t : form->m_tabs ) {
		// this tab has no elements.
		if( t->m_elements.empty( ) )
			continue;

		// get tab title.
		std::string title = t->m_title;

		// iterate tab elements.
		for( const auto e : t->m_elements ) {
			if( !( e->m_flags & ElementFlags::SAVE ) )
				continue;

			// get element type.
			size_t type = e->m_type;

			// get file identifier.
			std::string name = e->m_file_id;

			// NOTE; gross C++ casts ahead.
			// pls dont kill me its the best way.

			switch( type ) {

			case ElementTypes::CHECKBOX:
				config[ title ][ name ] = static_cast< Checkbox* >( e )->get( );
				break;

			case ElementTypes::SLIDER:
				config[ title ][ name ] = static_cast< Slider* >( e )->get( );
				break;

			case ElementTypes::KEYBIND:
				config[ title ][ name ] = static_cast< Keybind* >( e )->get( );
				break;

			case ElementTypes::DROPDOWN:
				config[ title ][ name ] = static_cast< Dropdown* >( e )->get( );
				break;

			case ElementTypes::COLORPICKER:
			{
				// get color
				Color color = static_cast< Colorpicker* >( e )->get( );

				// construct small array with color.
				std::array< int, 4 > arr = { color.r( ), color.g( ), color.b( ), color.a( ) };

				// write array to config.
				config[ title ][ name ] = arr;
				break;
			}

			case ElementTypes::MULTI_DROPDOWN:
				config[ title ][ name ] = static_cast< MultiDropdown* >( e )->GetActiveIndices( );
				break;

			case ElementTypes::EDIT:
				config[ title ][ name ] = static_cast< Edit* >( e )->get( );
				break;

			default:
				break;
			}
		}
	}

    return config_store::write( m_path / name, config );
}

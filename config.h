#pragma once

class Config {
public:
    void init( );
    bool LoadHotkeys( );
    bool SaveHotkeys( );
    bool load( const Form* form, const std::string& name );
    bool save( const Form* form, const std::string& name );

private:
    bool read( const std::string& name, nlohmann::json& value ) const;
    bool m_init = false;
    bool m_loading = false;
    std::filesystem::path m_path;
    std::filesystem::path m_legacy_path;
};

extern Config g_config;

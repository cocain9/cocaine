#include "support.h"
#include "config_under_test.cpp"
#include "print_under_test.cpp"
#include <iostream>

int callbacks = 0;
void count_callback() { ++callbacks; }
void save_keys() { g_config.SaveHotkeys(); }
void write_raw( const std::filesystem::path& path, const std::string& data ) {
    std::ofstream stream(path, std::ios::binary);
    stream << data;
}

int main( int argc, char** argv ) {
    assert(argc == 2);
    const std::filesystem::path root = argv[1];
    test_documents = root.string();
    std::filesystem::create_directories(root);
    using nlohmann::json;
    g_cl.print("100% player %s %n %d");
    assert(printed == "[Cocaine] 100% player %s %n %d");
    Input input{};
    for( int vk : {-100, -1, 256, 9999} ) {
        input.SetDown(vk); input.SetUp(vk);
        assert(!input.GetKeyState(vk) && !input.GetKeyPress(vk));
    }
    input.SetDown(255); input.update();
    assert(input.GetKeyState(255) && input.GetKeyPress(255));
    input.SetUp(255); assert(!input.GetKeyState(255));
    // Known Base64 vectors and every byte, including embedded NUL.
    for( const auto& pair : std::vector<std::pair<std::string, std::string>>{
        {"", ""}, {"f", "Zg=="}, {"fo", "Zm8="}, {"foo", "Zm9v"}, {"foobar", "Zm9vYmFy"}} ) {
        assert(crypto::base64_encode(pair.first) == pair.second);
        assert(crypto::base64_decode(pair.second) == pair.first);
    }
    std::string bytes;
    for( int i = 0; i < 256; ++i ) bytes += static_cast<char>(i);
    assert(crypto::base64_decode(crypto::base64_encode(bytes)) == bytes);

    json value;
    const auto file = root / "store.cfg";
    assert(!config_store::read(file, value));
    assert(config_store::write(file, json{{"key", 42}}));
    assert(config_store::read(file, value) && value["key"] == 42);
    assert(config_store::write(file, json{{"key", 7}}));
    assert(config_store::read(file, value) && value["key"] == 7);
    // Failed temporary-file creation preserves the previous complete file.
    std::filesystem::create_directory(root / "store.cfg.tmp");
    assert(!config_store::write(file, json{{"key", 99}}));
    assert(config_store::read(file, value) && value["key"] == 7);
    std::filesystem::remove(root / "store.cfg.tmp");
    for( const auto& data : std::vector<std::string>{"", "!???", "====", "e30=TRAILING",
        crypto::base64_encode("{"), crypto::base64_encode("[]"), crypto::base64_encode("null"),
        std::string(config_store::max_file_size + 1, 'A')} ) {
        write_raw(file, data);
        assert(!config_store::read(file, value));
    }
    for( const auto& name : {"", "..", "../escape.cfg", "..\\escape.cfg", "C:foo", "bad.", "bad "} )
        assert(!config_store::valid_filename(name));
    assert(config_store::valid_filename("1.cfg"));
    assert(!config_store::integer(json(std::numeric_limits<std::uint64_t>::max()), -1, 255));
    assert(!config_store::integer(json(0u), 1, 255));
    assert(!config_store::integer(json(1.5), 0, 255));

    Dropdown dropdown;
    dropdown.setup("dropdown", "dropdown", {"a", "b"});
    dropdown.SetCallback(count_callback);
    callbacks = 0;
    dropdown.set(1); dropdown.set(1); dropdown.set(9000);
    assert(dropdown.get() == 1 && callbacks == 1);
    dropdown.setup("dropdown", "dropdown", {"only"});
    assert(dropdown.get() == 0 && dropdown.GetActiveItem() == "only");
    MultiDropdown multi;
    multi.setup("multi", "multi", {"a", "b"});
    multi.SetCallback(count_callback);
    callbacks = 0;
    multi.select(1); multi.select(1); multi.select(100);
    assert(multi.GetActiveIndices().size() == 1 && callbacks == 1);
    multi.setup("multi", "multi", {"only"});
    assert(multi.empty());
    Slider slider;
    slider.setup("slider", "slider", 0, 10);
    slider.SetCallback(count_callback);
    callbacks = 0;
    slider.set(20); slider.set(30); slider.set(std::numeric_limits<float>::quiet_NaN());
    assert(slider.get() == 10 && callbacks == 1);
    Keybind key;
    key.set(32); key.set(-2); key.set(999);
    assert(key.get() == 32);
    struct TestEdit : Edit { void text(const std::string& value) { m_text = value; } };
    TestEdit edit;
    edit.setup("edit", "edit");
    edit.set(42); assert(edit.get() == 42);
    edit.set(-1); assert(edit.get() == -1);
    for( const auto text : {"not a number", "99999999999999999", "12abc"} ) {
        edit.text(text); assert(edit.get() == -1);
    }
    edit.set(-1);

    // Integration: real Config methods, real menu controls, synthetic Documents directory.
    g_config.init();
    Form form;
    Tab tab;
    tab.SetTitle("test");
    form.RegisterTab(&tab);
    Checkbox checkbox;
    checkbox.setup("checkbox", "checkbox");
    Colorpicker color;
    color.setup("color", "color", Color(1, 2, 3, 4));
    tab.RegisterElement(&checkbox);
    tab.RegisterElement(&dropdown);
    tab.RegisterElement(&multi);
    tab.RegisterElement(&slider);
    tab.RegisterElement(&key);
    key.setup("key", "key", 32);
    tab.RegisterElement(&color);
    tab.RegisterElement(&edit);
    assert(g_config.save(&form, "1.cfg"));
    checkbox.set(true); slider.set(5);
    assert(g_config.load(&form, "1.cfg"));
    assert(!checkbox.get() && slider.get() == 10);
    const auto cfg = root / "Cocaine" / "1.cfg";
    for( const auto& bad : std::vector<json>{
        {{"test", {{"checkbox", true}, {"slider", "bad"}}}},
        {{"test", {{"key", -5}}}}, {{"test", {{"color", {1,2,3}}}}},
        {{"test", {{"color", {1,2,3,256}}}}}, {{"test", {{"multi", {-1}}}}},
        {{"test", 1}}, {{"test", {{"edit", std::numeric_limits<std::uint64_t>::max()}}}}} ) {
        assert(config_store::write(cfg, bad));
        assert(!g_config.load(&form, "1.cfg"));
        assert(!checkbox.get() && slider.get() == 10); // no partial application
    }
    assert(!g_config.load(nullptr, "1.cfg"));
    assert(!g_config.save(&form, "../escape.cfg"));
    std::filesystem::create_directories(root / "user");
    assert(config_store::write(root / "user" / "2.sup", json{{"test", {{"checkbox", true}}}}));
    assert(g_config.load(&form, "2.cfg") && checkbox.get());
    write_raw(root / "Cocaine" / "2.cfg", "broken");
    assert(!g_config.load(&form, "2.cfg")); // never silently falls back from corrupt new config

    const std::array<Keybind*, 6> keys{&g_menu.main.config.key1, &g_menu.main.config.key2,
        &g_menu.main.config.key3, &g_menu.main.config.key4, &g_menu.main.config.key5, &g_menu.main.config.key6};
    json hotkeys = json::object();
    for( std::size_t i = 0; i < keys.size(); ++i ) {
        const auto id = "key" + std::to_string(i);
        keys[i]->setup(id, id);
        keys[i]->SetCallback(save_keys);
        hotkeys[id] = static_cast<int>(i + 30);
    }
    const auto key_file = root / "Cocaine" / "keys.cfg";
    assert(config_store::write(key_file, hotkeys));
    assert(g_config.LoadHotkeys());
    assert(config_store::read(key_file, value) && value == hotkeys);
    for( std::size_t i = 0; i < keys.size(); ++i ) assert(keys[i]->get() == static_cast<int>(i + 30));
    hotkeys["key5"] = 999;
    hotkeys["key0"] = 12;
    assert(config_store::write(key_file, hotkeys));
    assert(!g_config.LoadHotkeys() && keys[0]->get() == 30);
    assert(g_config.SaveHotkeys());
    std::cout << "All Cocaine regression checks passed.\n";
}

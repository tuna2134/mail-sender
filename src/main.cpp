#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <jinja2cpp/template.h>

int main() {
    using json = nlohmann::json;
    std::ifstream f("config.json");
    json config = json::parse(f);
    std::string tmp_path  = config["template_file"].get<std::string>();
    std::cout << tmp_path << std::endl;

    return 0;
}
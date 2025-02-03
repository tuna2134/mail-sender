#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <jinja2cpp/template.h>
#include "cpp/opportunisticsecuresmtpclient.hpp"
#include "cpp/plaintextmessage.hpp"

int main() {
    using json = nlohmann::json;
    std::ifstream f("config.json");
    json config = json::parse(f);

    using namespace jed_utils::cpp;
    OpportunisticSecureSMTPClient client(
        config["smtp"]["address"].get<std::string>(),
        config["smtp"]["port"].get<int>()
    );
    client.setCredentials(Credential("myfromaddress@test.com", "mypassword"));

    std::string tmp_path  = config["template_file"].get<std::string>();
    std::ifstream ft(tmp_path);
    std::string line;
    std::string tpl_contents;
    while (std::getline(ft, line)) {
        tpl_contents += line;
        tpl_contents.push_back('\n');
    }
    jinja2::Template tpl;
    tpl.Load(tpl_contents);

    for (const json& user : config["target_users"]) {
        jinja2::ValuesMap values;
        values["username"] = user["username"].get<std::string>();
        std::string result = tpl.RenderAsString(values).value();
        std::cout << result << std::endl;
    }

    return 0;
}
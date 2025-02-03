#include <jinja2cpp/template.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "cpp/opportunisticsecuresmtpclient.hpp"
#include "cpp/plaintextmessage.hpp"

int main() {
    using json = nlohmann::json;
    std::ifstream f("config.json");
    json config = json::parse(f);

    using namespace jed_utils::cpp;
    OpportunisticSecureSMTPClient client(
        config["smtp"]["address"].get<std::string>(),
        config["smtp"]["port"].get<int>());
    client.setCredentials(Credential(
        config["smtp"]["credentials"]["username"].get<std::string>(),
        config["smtp"]["credentials"]["password"].get<std::string>()));

    std::string tmp_path = config["template_file"].get<std::string>();
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
        MessageAddress address =
            MessageAddress(config["email"].get<std::string>(),
                           config["username"].get<std::string>());
        try {
            PlaintextMessage msg(
                address,
                {MessageAddress(user["email"].get<std::string>(),
                                user["username"].get<std::string>())},
                config["title"].get<std::string>(), result);
            int err_no = client.sendMail(msg);
            if (err_no != 0) {
                std::cerr << client.getCommunicationLog() << '\n';
                std::string errorMessage = client.getErrorMessage(err_no);
                std::cerr << "An error occurred: " << errorMessage
                          << " (error no: " << err_no << ")" << '\n';
                return 1;
            }
            std::cout << client.getCommunicationLog() << '\n';
            std::cout << "Operation completed!" << std::endl;
        } catch (std::invalid_argument& err) {
            std::cerr << err.what() << std::endl;
        }
        std::cout << result << std::endl;
    }

    return 0;
}
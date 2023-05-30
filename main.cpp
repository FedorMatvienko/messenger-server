#include <iostream>
#include <filesystem>
#include <fstream>
#include <jsoncpp/json/json.h>
using namespace std;
int main()
{
    cout << "server" << endl;
    Json::Value config;
    std::filesystem::path path = std::filesystem::current_path() / "server.json";
    cout << path << endl;
    std::ifstream file_json(path.string());
    file_json >> config;
    string s1 = config["first"].asString();
    string s2 = config["second"].asString();
    cout << s1 << " " << s2 << endl;
    return 0;
}
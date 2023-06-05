#include <iostream>
#include <filesystem>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <string>

#include <codecvt>
#include <locale>

//Файлы для работы с сокетами
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <cwchar>

using namespace std;

class workF
{
    public:
    std::wofstream file;
    void print ( wstring s )
    {
        file << s.c_str() << std::endl;
    }
    workF()
    {
        std::filesystem::path path = std::filesystem::current_path() / "output.log";
        file.open("../../output.log",std::wofstream::app);
        file.imbue(std::locale(""));
    }
    ~workF()
    {
        file.close();
    }
};

int main()
{
    std::string local = setlocale(LC_ALL, "");
    std::wstring local_name(local.begin(),local.end());
    std::wcout << L"Локаль: " << local_name << std::endl;
    workF f;
    std::wstring str;

    // cout << "server" << endl;
    // Json::Value config;
    // std::filesystem::path path = std::filesystem::current_path() / "server.json";
    // cout << path << endl;
    // std::ifstream file_json(path.string());
    // file_json >> config;
    // string s1 = config["first"].asCString();
    // string s2 = config["second"].asCString();
    // cout << s1 << " " << s2 << endl;
    
    int sockfd = 0;
    int new_socket = 0;
    int opt = 1;
    socklen_t optlen=sizeof(opt);
    int valread = 0;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    string buf="";
    string hello = "Hello from server";
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        wcout << L"Ошибка создания сокета" << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        wcout << L"Сокет создан" << endl;
    }

    if ( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_KEEPALIVE, &opt, sizeof(opt)) )
    {
        wcout << L"Сокет уже используется" << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        wcout << L"Сокет не используется повторно" << endl;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if ( bind(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        wcout << L"Ошибка привязывания сокета к адресу" << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        wcout << L"Сокет привязан к адресу" << endl;
    }
    if ( listen(sockfd, 3) < 0)
    {
        wcout << L"Переполнение очереди" << endl;
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
    {
        wcout << L"" << endl;
        exit(EXIT_FAILURE);
    }
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    while ( str != L"-exit" )
    {
        string s;
        memset( buffer, 0x00, 1024 );
        valread = read(new_socket, buffer, 1024);
        s = buffer;
        str = converter.from_bytes(s);
        f.print( str );
        if ( send(new_socket, hello.c_str(), hello.size()+1, 0) == -1 )
        {
            break;
        }
        wcout << str << endl;
        str.clear();
    }

    send (new_socket, hello.c_str(), hello.size(), 0);
    close(new_socket);
    shutdown(sockfd, SHUT_RDWR);
    return 0;
}
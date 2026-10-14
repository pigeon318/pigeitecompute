#include <array>
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <nlohmann/json.hpp>
#include <thread>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#endif


using boost::asio::ip::tcp;
using json = nlohmann::json;


std::string get_os()
{
#if defined(_WIN32)
    return "windows";
#elif defined(__APPLE__)
    return "macos";
#elif defined(__linux__)
    return "linux";
#elif defined(__FreeBSD__)
    return "freebsd";
#else
    return "unknown";
#endif
}

std::string get_architecture()
{
#if defined(__x86_64__) || defined(_M_X64)
    return "x86_64";
#elif defined(__i386__) || defined(_M_IX86)
    return "x86";
#elif defined(__aarch64__) || defined(_M_ARM64)
    return "arm64";
#elif defined(__arm__) || defined(_M_ARM)
    return "arm32";
#elif defined(__riscv) && (__riscv_xlen == 64)
    return "riscv64";
#else
    return "unknown";
#endif
}

uint64_t get_total_memory_mb()
{
#ifdef _WIN32
    MEMORYSTATUSEX memory{};
    memory.dwLength = sizeof(memory);

    if (!GlobalMemoryStatusEx(&memory))
        return 0;

    return memory.ullTotalPhys / (1024ULL * 1024ULL);
#else
    return 0;
#endif
}

int main(int argc, char* argv[])
{

    if (argc != 2)
{
    std::cerr << "Usage: pigeite-node <server-address>\n";
    return 1;
}
    
    std::string server_address = argv[1];
    boost::asio::io_context io_context;
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "31820");
    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    std::cout << "connected to" << server_address << ":31820\n";

    std::string hello =
    "{\"type\":\"hello\",\"protocol\":0,"
    "\"client\":\"pigeite-node\","
    "\"client_version\":\"0.0.1\"}\n";

    boost::asio::write(socket, boost::asio::buffer(hello));

     boost::asio::streambuf buffer;

    boost::asio::read_until(socket, buffer, '\n');

    std::istream input(&buffer);

    std::string message;
    std::getline(input, message);
    json recived = json::parse(message);

    std::cout << recived << '\n';

    if(!recived.contains("type")){
        std::cerr << "invalid message, missing type";
        return 1;
    }
    if(!recived["type"].is_string()){
        std::cerr << "invalid message, type isnt a string";
        return 1;
    }
    if(recived["type"] != "hello_ack"){
        std::cerr << "invalid message, excpected hello_ack";
        return 1;
    }
    if(recived["protocol"] != 0){
        std::cerr << "wrong protocol version";
        return 1;
    }
    if(!recived.contains("node_id")){
        std::cerr << "invalid message, missing node_id";
        return 1;
    }
    if(!recived["node_id"].is_string()){
        std::cerr << "invalid message, node_id isnt a string";
        return 1;
    }
    if(recived["node_id"].get<std::string>().empty()){
        std::cerr << "invalid message, node_id is empty";
        return 1;
    }

    std::string node_id = recived["node_id"].get<std::string>();

    std::cout << "recived valid HELLO_ACK\n";
    std::cout << "asigned node id: " << node_id << '\n';

    unsigned int threads = std::thread::hardware_concurrency();

    std::string capabilities =
    "{\"type\":\"capabilities\","
    "\"architecture\":\"" + get_architecture() + "\","
    "\"os\":\"" + get_os() + "\","
    "\"logical_threads\":" + std::to_string(std::thread::hardware_concurrency()) + ","
    "\"memory_mb\":" + std::to_string(get_total_memory_mb()) + ","
    "\"workloads\":[\"pi.monte_carlo\"]}\n";

    boost::asio::write(socket, boost::asio::buffer(capabilities));

    std::cout << "sent CAPABILITIES";


   



}
#include <array>
#include <iostream>
#include <boost/asio.hpp>
#include <string>


using boost::asio::ip::tcp;

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

    std::cout << "connected to" << server_address << ":31820";

}
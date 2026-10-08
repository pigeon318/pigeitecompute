#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main()
{
    try{
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(
            io_context,
            tcp::endpoint(tcp::v4(), 31820)
        );

        std::cout << "listening on 31820\n";

        for (;;){
            tcp::socket socket(io_context);

            acceptor.accept(socket);

            std::cout << "node connected.\n";
        }
    }
    catch (std::exception& e){
        std::cerr << "Server error: " << e.what() << '\n';
        return 1;
    }
}
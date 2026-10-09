#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

using boost::asio::ip::tcp;
using json = nlohmann::json;

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

            boost::asio::streambuf buffer;

            boost::asio::read_until(socket, buffer, '\n');

            std::istream input(&buffer);

            std::string message;
            std::getline(input, message);

            std::cout << message;
            json recived = json::parse(message);

            std::cout << recived;

            if (!recived.contains("type")){
                std::cerr << "invalid message, missing type \n";
                return 1;
            }
            if (!recived["type"].is_string()){
                std::cerr << "invalid message, type musy be a string\n";
                return 1;
            }
            if (recived["type"] != "hello"){
                std::cerr << "invalid message, excpected hello\n";
                return 1;
            }
            if (recived["protocol"] != 0){
                std::cerr << "wrong protocol version";
                return 1;
            }



        }
    }
    catch (std::exception& e){
        std::cerr << "Server error: " << e.what() << '\n';
        return 1;
    }
}
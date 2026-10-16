#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <sstream>
#include <iomanip>


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

        unsigned int next_node_id = 1;

        for (;;){
            tcp::socket socket(io_context);

            acceptor.accept(socket);

            std::cout << "node connected.\n";

            boost::asio::streambuf buffer;

            boost::asio::read_until(socket, buffer, '\n');

            std::istream input(&buffer);

            std::string message;
            std::getline(input, message);
            json recived = json::parse(message);

            std::cout << recived << '\n';

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
                std::cerr << "wrong protocol version\n";
                return 1;
            }
            else{
                std::cout << "recived valid HELLO\n";
            }

            
            std::ostringstream id_stream;
            id_stream << "n" << std::setw(6) << std::setfill('0') << next_node_id++;
            std::string node_id = id_stream.str();

            std::string hello_ack =
                "{\"type\":\"hello_ack\",\"protocol\":0,"
                "\"server_version\":\"0.0.1\","
                "\"node_id\":\"" + node_id + "\"}\n";

            boost::asio::write(socket, boost::asio::buffer(hello_ack));
            std::cout << "senrt HELLO_ACK\n";

            boost::asio::read_until(socket, buffer, '\n');

            message.clear();
            std::getline(input, message);
            recived = json::parse(message);
            std::cout << recived << '\n';

        
            if(!recived["type"].is_string()){
                std::cerr << "invalid message, type must be string";
            }
            if(recived["type"] != "capabilities"){
                std::cerr << "invalid message, type must be capabilities";
            }
            if(!recived["architecture"].is_string()){
                std::cerr << "invalid message, architechture must be a string";
            }
            if(recived["architecture"].get<std::string>().empty()){
                std::cerr << "invalid message, architechture must not be empty";
            }
            if(!recived["os"].is_string()){
                std::cerr << "invalid message, os must be a string";
            }
            else{
                std::cout << "recived valid CAPABILITIES";
            }

            json node_capbilities = recived;

            boost::asio::read_until(socket, buffer, '\n');

            message.clear();
            std::getline(input, message);
            recived = json::parse(message);
            std::cout << recived << '\n';

            if(recived["type"] != "ready"){
                std::cerr << "invalid message, excpected type to be ready";
            }
            else{
                std::cout << "recived valid READY node is now ready to work";
            }






        }
    }
    catch (std::exception& e){
        std::cerr << "Server error: " << e.what() << '\n';
        return 1;
    }
}
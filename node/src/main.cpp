#include <array>
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <nlohmann/json.hpp>


using boost::asio::ip::tcp;
using json = nlohmann::json;

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

}
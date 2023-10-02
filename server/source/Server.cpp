#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/bind.hpp>
#include <cstring>
#include <exception>
#include <iostream>
#include "../include/Server.hpp"

using boost::asio::ip::udp;

void udp_server::handle_send(const boost::system::error_code &, std::size_t bytes_transferred)
{
    std::cout << "bytes_transferred = " << bytes_transferred << std::endl;
    start_receive();
}

void udp_server::handle_receive(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (!error || error == boost::asio::error::message_size) {
        Message type;
        std::memcpy(&type, &_recv_buffer, sizeof(Message));
        test_struct test;
        test.type = 2;
        test.id = 7;
        test.pos = {1, 13};
        _socket.async_send_to(boost::asio::buffer(&test, sizeof(test)), _remote_point,
            boost::bind(&udp_server::handle_send, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
}

void udp_server::start_receive()
{
    _socket.async_receive_from(
        boost::asio::buffer(_recv_buffer), _remote_point,
        boost::bind(&udp_server::handle_receive, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

udp_server::udp_server(boost::asio::io_service & svc, std::size_t port) : _socket(svc, udp::endpoint(udp::v4(), port))
{
    start_receive(); 
}

udp_server::~udp_server()
{
    _socket.close();
}

int main()
{
    try {
        boost::asio::io_context svc;
        udp_server server(svc, 5000);
        svc.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

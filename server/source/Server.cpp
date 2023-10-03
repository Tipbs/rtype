#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/bind.hpp>
#include <cstddef>
#include <cstring>
#include <deque>
#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include "../include/Server.hpp"

using boost::asio::ip::udp;

typedef std::deque<boost::asio::const_buffer> ConstBuffers;

void udp_server::handle_send(const boost::system::error_code &error, std::size_t bytes_transferred) //Callback to the send function 
{
    if (!error) {
        std::cout << "bytes_transferred = " << bytes_transferred << std::endl;
        start_receive();
    }
}

void udp_server::handle_broadcast(const boost::system::error_code &error, std::size_t bytes_transferred) //Callback to broadcast
{
    if (!error) {
        std::cout << "bytes_transferred = " << bytes_transferred << std::endl;
    }
}

void udp_server::multiple_broadcast(std::vector<udp::endpoint> tmp, std::map<std::size_t, std::vector<UserCmd>> commands)
{
    Message test;
    test.type = 1;
    for (const auto& client_endpoint : tmp) {
        _socket.async_send_to(boost::asio::buffer(&test, sizeof(test)), client_endpoint,
            boost::bind(&udp_server::handle_broadcast, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
}

void udp_server::broadcast() // Broadcast a message to all connected clients that already sent a message 
{
    std::vector<udp::endpoint> tmp = clients;
    std::thread broadcasting(&udp_server::multiple_broadcast, this, tmp, std::move(cmd));
    broadcasting.detach();
}

void udp_server::add_buff_to_cmd()
{
    std::size_t i = 0;
    for (const auto &client_endpoint : clients)
        i++;
    UserCmd tmp;
    std::memcpy(&tmp, &_recv_buffer, sizeof(UserCmd));
    cmd[i - 1].push_back(tmp);
}

void udp_server::handle_receive(const boost::system::error_code &error, std::size_t bytes_transferred) // Callback to the receive function
{
    if (!error || error == boost::asio::error::message_size) {
        std::cout << "Received " << bytes_transferred << "bytes" << std::endl;
        if (std::find(clients.begin(), clients.end(), _remote_point) == clients.end() && clients.size() <= 4) {
            clients.push_back(_remote_point);
        }
        add_buff_to_cmd();
        broadcast();
        start_receive();
    }
}

void udp_server::start_receive() // Receive function
{
    _socket.async_receive_from(
        boost::asio::buffer(_recv_buffer), _remote_point,
        boost::bind(&udp_server::handle_receive, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

udp_server::udp_server(std::size_t port) : _svc(), _socket(_svc, udp::endpoint(udp::v4(), port))
{
    _port = port;
    _socket.non_blocking(true);
    start_receive(); 
    _svc.run();
}

udp_server::~udp_server()
{
    _socket.close();
}

int main(int ac, char **av)
{
    try {
        std::size_t port = 5000;
        if (ac == 2 && std::stoi(av[1]))
            port = std::stoi(av[1]);
        udp_server server(port);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

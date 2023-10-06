#include <cstddef>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/registered_buffer.hpp>
#include <boost/bind.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "../include/Client.hpp"

using boost::asio::ip::udp;
std::binary_semaphore MainToThread{0};
std::binary_semaphore ThreadToMain{0};

void udp_client::handle_send(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {

    }
}

void udp_client::send()
{
    std::ostringstream oss;
    boost::archive::binary_oarchive archive(oss);
    archive << cmd;
    std::string serializedData = oss.str();
    std::cout << "Sending " << serializedData.size() << "bytes from " << serializedData.data() << std::endl;
    _socket.async_send_to(boost::asio::buffer(serializedData.c_str(), serializedData.size()), _remote_point,
        boost::bind(&udp_client::handle_send, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void udp_client::handle_receive(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        std::cout << "Received " << bytes_transferred << " bytes" << std::endl;
        start_receive();
    }
}

void udp_client::start_receive()
{
    _socket.async_receive_from(boost::asio::buffer(_recv_buffer), _remote_point,
        boost::bind(&udp_client::handle_receive, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void udp_client::handle_tick()
{
    MainToThread.release();
    ThreadToMain.acquire();
    timer.expires_from_now(boost::posix_time::millisec(50));
    timer.async_wait(boost::bind(&udp_client::handle_tick, this));
}

void udp_client::send_user()
{
    while (1) {
        MainToThread.acquire();
        cmd_mutex.lock();
        send();
        cmd_mutex.unlock();
        ThreadToMain.release();
    }
}

udp_client::udp_client(boost::asio::io_context &_svc,const std::string &ip, const std::string &port) : _socket(_svc, udp::v4()), timer(_svc)
{
    udp::resolver resolver(_svc);
    udp::resolver::query query(udp::v4(), ip, port);
    udp::resolver::iterator iter = resolver.resolve(query);
    _remote_point = *iter;
    tick = std::thread(&udp_client::handle_tick, this); //Timer thread
    sending = std::thread(&udp_client::send_user, this); // Snapshot thread
    tick.detach();
    sending.detach();
    cmd.reset();
    cmd.moved.x = 5;
    cmd.moved.y = 2;
    send();
    start_receive();
}

udp_client::~udp_client()
{
    _socket.close();
}

int main(int ac, char **av)
{
    try {
        boost::asio::io_context _svc;
        udp_client client(_svc, av[1], av[2]);
        _svc.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

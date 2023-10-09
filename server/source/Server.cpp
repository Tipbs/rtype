#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/bind.hpp>
#include <exception>
#include <iostream>
#include <istream>
#include <map>
#include <thread>
#include <semaphore>
#include "../include/Server.hpp"

using boost::asio::ip::udp;
std::binary_semaphore MainToThread{0};
std::binary_semaphore ThreadToMain{0};

void udp_server::handle_send(const boost::system::error_code &error, std::size_t bytes_transferred) //Callback to the send function 
{
    if (!error) {
        start_receive();
    }
}

void udp_server::handle_broadcast(const boost::system::error_code &error, std::size_t bytes_transferred) //Callback to broadcast
{
    if (!error) {
    }
}

void udp_server::handle_check(const boost::system::error_code &error)
{
    if (!error) {
        std::cout << "check" << std::endl;
        auto now = boost::posix_time::microsec_clock::universal_time();
        for (auto it = clients.begin(); it != clients.end(); ) {
            if ((now - it->second).total_seconds() > 5) {
                std::cout << "Client " << it->first << " disconnected\n";
                it = clients.erase(it);
            } else {
                ++it;
            }
        }
        start_check();
    }
}

void udp_server::start_check()
{
    check_timer.expires_from_now(boost::posix_time::seconds(1));
    check_timer.async_wait(boost::bind(&udp_server::handle_check, this,
        boost::asio::placeholders::error));
}

void start_snapshot(UserCmd state, std::size_t id)
{

}

void udp_server::multiple_broadcast(std::map<udp::endpoint, boost::posix_time::ptime> tmp, std::map<std::size_t, std::vector<UserCmd>> commands)
{
    Message test;
    test.type = 1;
    for (const auto &client : commands) {
        for (const auto &client_command : client.second) {
            start_snapshot(client_command, client.first);
        }
    }

    for (const auto& client_endpoint : tmp) {
        _socket.async_send_to(boost::asio::buffer(&test, sizeof(test)), client_endpoint.first,
            boost::bind(&udp_server::handle_broadcast, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }
}

void run_system()
{

}

void udp_server::broadcast() // Broadcast a message to all connected clients that already sent a message 
{
    while (1) {
        MainToThread.acquire();
        std::map<udp::endpoint, boost::posix_time::ptime> tmp = clients;
        run_system();
        ThreadToMain.release();
        cmd_mutex.lock();
        multiple_broadcast(tmp, std::move(cmd));
        cmd_mutex.unlock();
    }
}

void udp_server::handle_tick() //tick every seconds
{
    MainToThread.release();
    ThreadToMain.acquire();
    tick_timer.expires_from_now(boost::posix_time::millisec(50));
    tick_timer.async_wait(boost::bind(&udp_server::handle_tick, this));
}

void udp_server::deserialize(const std::size_t bytes_transferred)
{
    std::size_t i = 1;
    for (const auto &client_endpoint : clients) {
        if (client_endpoint.first == _remote_point)
            break;
        i++;
    }
    std::string seralizedData(_recv_buffer.data(), bytes_transferred);
    std::istringstream iss(seralizedData);
    boost::archive::binary_iarchive archive(iss);
    UserCmd tmp;
    archive >> tmp;
    cmd_mutex.lock();
    cmd[i].push_back(tmp);
    cmd_mutex.unlock();
}

void udp_server::handle_receive(const boost::system::error_code &error, std::size_t bytes_transferred) // Callback to the receive function
{
    if (!error || error == boost::asio::error::message_size) {
        std::cout << "Received " << bytes_transferred << "bytes" << std::endl;
        if (clients.count(_remote_point) > 0 || clients.size() <= 4) {
            clients[_remote_point] = boost::posix_time::microsec_clock::universal_time();
            deserialize(bytes_transferred);
        }
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

udp_server::udp_server(std::size_t port) : _svc(), _socket(_svc, udp::endpoint(udp::v4(), port)), tick_timer(_svc), check_timer(_svc)
{
    _port = port;
    tick = std::thread(&udp_server::handle_tick, this); //Timer thread
    broadcasting = std::thread(&udp_server::broadcast, this); // Snapshot thread
    tick.detach();
    broadcasting.detach();


    start_receive(); 
    start_check();
    _svc.run();
}

udp_server::~udp_server()
{
    _socket.close();
    tick.std::thread::~thread();
    broadcasting.std::thread::~thread();
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

#include "Server.hpp"
#include <exception>
#include <iostream>
#include <istream>
#include <map>
#include <semaphore>
#include <boost/archive/archive_exception.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/bind/bind.hpp>
#include "../../shared/Component.hpp"
#include "../../shared/Bundle.hpp"
#include <syncstream>
#include "ServerBundle.hpp"

using boost::asio::ip::udp;
std::binary_semaphore MainToThread {0};
std::binary_semaphore ThreadToMain {0};

void udp_server::handle_broadcast(
    const boost::system::error_code &error,
    std::size_t bytes_transferred) // Callback to broadcast
{
    if (!error) {}
}

void udp_server::handle_check(const boost::system::error_code &error)
{
    if (!error) {
        auto now = boost::posix_time::microsec_clock::universal_time();
        std::osyncstream(std::cout) << "check\n";
        for (auto it = clients.begin(); it != clients.end();) {
            if ((now - it->second._timer).total_seconds() > 5) {
                std::osyncstream(std::cout) << "Client " << it->first << " disconnected\n";
                std::osyncstream(std::cout) << "Killed entity " << it->second._id << std::endl;
                reg.kill_entity(reg.entity_from_index(it->second._id));
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
    check_timer.async_wait(boost::bind(
        &udp_server::handle_check, this, boost::asio::placeholders::error));
}

void udp_server::multiple_broadcast(
    std::map<udp::endpoint, struct Clients> tmp, std::vector<NetEnt> netent)
{
    std::ostringstream oss;
    boost::archive::binary_oarchive archive(oss);
    archive << netent;
    std::string serializedData = oss.str();
    try {
        std::ostringstream oss;
        boost::archive::binary_oarchive archive(oss);
        archive << netent;
        std::string serializedData = oss.str();
        for (const auto &client_endpoint : tmp) {
            if (client_endpoint.second.isClientConnected == false)
                continue;
            _socket.async_send_to(
                boost::asio::buffer(
                    serializedData.c_str(), serializedData.size()),
                client_endpoint.first,
                boost::bind(
                    &udp_server::handle_broadcast, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }
    } catch (boost::archive::archive_exception &e) {
        std::osyncstream(std::cout) << "Archive Error: " << e.what() << std::endl;
    }
}

void udp_server::run_system()
{
    reg.user_cmds = std::move(cmd);
    reg.run_systems();
}

void udp_server::start_snapshot() // Broadcast a message to all connected
                                  // clients that already sent a message
{
    while (1) {
        MainToThread.acquire();
        auto tmp = clients;
        cmd_mutex.lock();
        run_system();
        std::vector<NetEnt> netent = std::move(reg._netent);
        cmd_mutex.unlock();
        multiple_broadcast(tmp, netent);
        ThreadToMain.release();
    }
}

void udp_server::handle_tick() // tick every seconds
{
    while (true) {
        MainToThread.release();
        ThreadToMain.acquire();
        tick_timer.expires_from_now(boost::posix_time::millisec(50));
        ResetFrameTime();
        tick_timer.wait();
    }
}

void udp_server::deserialize(const std::size_t bytes_transferred)
{
    try {
        std::string seralizedData(_recv_buffer.data(), bytes_transferred);
        std::istringstream iss(seralizedData);
        boost::archive::binary_iarchive archive(iss);
        UserCmd tmp;
        archive >> tmp;
        cmd_mutex.lock();
        cmd[clients[_remote_point]._id].push_back(tmp);
        cmd_mutex.unlock();
    } catch (boost::archive::archive_exception &e) {
        std::cerr << "deserialization failed: " << e.what() << std::endl;
    }
}

void udp_server::handle_send(
    const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {}
}

void udp_server::send_playerId(
    std::size_t playerId, udp::endpoint client_endpoint)
{
    Utils::PlayerId player;
    player.id = playerId;
    std::ostringstream oss;
    boost::archive::binary_oarchive archive(oss);
    archive << player;
    std::string serializedData = oss.str();
    _socket.async_send_to(
        boost::asio::buffer(serializedData.c_str(), serializedData.size()),
        client_endpoint,
        boost::bind(
            &udp_server::handle_send, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void udp_server::wait_for_connexion(std::size_t bytes_transferred)
{
    if (bytes_transferred != 1 && clients.size() == 0)
        return;
    if (bytes_transferred == 1 && clients.count(_remote_point) == 0) {
        size_t player = create_player_server(reg, Position(0, 0));
        if (clients.size() == 0)
            start_threads();
        clients[_remote_point]._id = (size_t) player;
        clients[_remote_point].isClientConnected = false;
        clients[_remote_point]._timer =
            boost::posix_time::microsec_clock::universal_time();
        send_playerId(clients[_remote_point]._id, _remote_point);
    } else if (bytes_transferred == 1 && clients.count(_remote_point) != 0) {
        clients[_remote_point]._timer =
            boost::posix_time::microsec_clock::universal_time();
        send_playerId(clients[_remote_point]._id, _remote_point);
    } else {
        clients[_remote_point].isClientConnected = true;
        clients[_remote_point]._timer =
            boost::posix_time::microsec_clock::universal_time();
        deserialize(bytes_transferred);
    }
}

void udp_server::handle_receive(
    const boost::system::error_code &error,
    std::size_t bytes_transferred) // Callback to the receive function
{
    if (!error || error == boost::asio::error::message_size) {
        std::osyncstream(std::cout) << "Received " << bytes_transferred << "bytes" << std::endl;
        if (clients.count(_remote_point) == 0) {
            wait_for_connexion(bytes_transferred);
            start_receive();
            return;
        }
        if (clients.count(_remote_point) > 0 || clients.size() <= 4) {
            if (clients[_remote_point].isClientConnected == false) {
                wait_for_connexion(bytes_transferred);
                start_receive();
                return;
            } else {
                clients[_remote_point]._timer =
                    boost::posix_time::microsec_clock::universal_time();
                deserialize(bytes_transferred);
            }
        }
        start_receive();
    }
}

void udp_server::start_receive() // Receive function
{
    _socket.async_receive_from(
        boost::asio::buffer(_recv_buffer), _remote_point,
        boost::bind(
            &udp_server::handle_receive, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void synchronize(Registry &reg, sparse_array<Direction> &directions)
{
    for (auto &player : reg.user_cmds) {
        auto &dir = directions[player.first];
        for (auto &cmds : player.second) {
            dir->dir_X += cmds.moved.x;
            dir->dir_Y += cmds.moved.y;
        }
    }
}

void extract(Registry &reg, sparse_array<Position> &positions)
{
    for (size_t ind = 0; ind < positions.size(); ind++) {
        auto &pos = positions[ind];
        if (!pos)
            continue;
        NetEnt tmp;
        tmp.id = ind;
        tmp.pos.x = pos->pos_X;
        tmp.pos.y = pos->pos_Y;
        reg._netent.push_back(tmp);
    }
}

void udp_server::start_threads()
{
    tick = std::thread(&udp_server::handle_tick, this); // Timer thread
    broadcasting =
        std::thread(&udp_server::start_snapshot, this); // Snapshot thread
    tick.detach();
    broadcasting.detach();
    start_check();
}

udp_server::udp_server(std::size_t port)
    : _svc(), _socket(_svc, udp::endpoint(udp::v4(), port)), tick_timer(_svc),
      check_timer(_svc)
{
    reg.register_component<Size>();
    reg.register_component<Position>();
    reg.register_component<Speed>();
    reg.register_component<Direction>();
    reg.register_component<SpawnGrace>();
    reg.register_component<Player>();
    reg.register_component<Damages>();
    reg.register_component<Health>();
    reg.add_system<Direction>(synchronize);
    reg.add_system<Position, Size, SpawnGrace, Damages, Health>(colision);
    reg.add_system<Position, Speed, Direction>(move);
    reg.add_system<Position>(extract);

    _port = port;

    start_receive();
    _svc.run();
}

udp_server::~udp_server()
{
    _socket.close();
    tick.std::thread::~thread();
    broadcasting.std::thread::~thread();
}

int helper()
{
    std::osyncstream(std::cout) << "USAGE\n";
    std::osyncstream(std::cout) << "\t./server <port>\n";
    std::osyncstream(std::cout) << " port\tport number of the server\n";
    return 0;
}

int main(int ac, char **av)
{
    try {
        std::size_t port = 5000;
        if (ac == 2 &&
            (std::string(av[1]) == "-h" || std::string(av[1]) == "--help"))
            return helper();
        if (ac == 2 && std::stoi(av[1]))
            port = std::stoi(av[1]);
        udp_server server(port);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

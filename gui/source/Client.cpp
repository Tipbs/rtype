#include "Client.hpp"
#include <exception>
#include <iostream>
#include <semaphore>
#include <sstream>
#include <string>
#include <syncstream>
#include <thread>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/bind.hpp>
#include "../../shared/NetEnt.hpp"

using boost::asio::ip::udp;
std::binary_semaphore MainToThread {0};
std::binary_semaphore ThreadToMain {0};

void udp_client::handle_send(
    const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {}
}

void udp_client::send()
{
    std::ostringstream oss;
    boost::archive::binary_oarchive archive(oss);
    // std::cout << "coucou je suis hardstuck\n";
    _reg.currentCmd.mutex.lock();
    auto copyCmd = _reg.currentCmd.cmd;
    _reg.currentCmd.cmd.reset();
    _reg.currentCmd.mutex.unlock();
    // std::osyncstream(std::cout) << "eeqsdqsd\n";
    archive << copyCmd;
    std::string serializedData = oss.str();
    // std::osyncstream(std::cout) << "Sending " << serializedData.size() <<
    // "bytes from " << serializedData.data() << std::endl;
    _socket.async_send_to(
        boost::asio::buffer(serializedData.c_str(), serializedData.size()),
        _remote_point,
        boost::bind(
            &udp_client::handle_send, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void udp_client::handle_receive(
    const boost::system::error_code &error, std::size_t bytes_transferred)
{
    // std::osyncstream(std::cout) << "Received mais erreur\n";
    if (!error) {
        std::osyncstream(std::cout) << "Received " << bytes_transferred << "bytes" << std::endl;
        try {
            std::string seralizedData(_recv_buffer.data(), bytes_transferred);
            std::istringstream iss(seralizedData);
            boost::archive::binary_iarchive archive(iss);
            std::vector<NetEnt> tmp;
            archive >> tmp;
            _reg.netEnts.mutex.lock();
            _reg.netEnts.ents.insert(
                _reg.netEnts.ents.begin(), tmp.begin(), tmp.end());
            if (_reg.netEnts.ents.size() == 1 &&
                (_reg.netEnts.ents[0].type == EntityType::Win ||
                 _reg.netEnts.ents[0].type == EntityType::Lose)) {
                state = notPlaying;
            }
            _reg.netEnts.mutex.unlock();
        } catch (std::exception &err) {
            std::osyncstream(std::cout)
                << "Error in handle_receive: " << err.what()
                << " (probably normal)\n";
        }
    }
    start_receive();
}

void udp_client::start_receive()
{
    _socket.async_receive_from(
        boost::asio::buffer(_recv_buffer), _remote_point,
        boost::bind(
            &udp_client::handle_receive, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void udp_client::handle_tick()
{
    while (true) {
        MainToThread.release();
        ThreadToMain.acquire();
        timer.expires_from_now(boost::posix_time::millisec(50));
        timer.wait();
    }
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

void udp_client::net_get_id(
    const boost::system::error_code &error, std::size_t bytes_transferred)
{
    // std::osyncstream(std::cout) << "Received mais erreur net_get_id ;)" <<
    // bytes_transferred
    //           << "\n";
    if (!error) {
        std::osyncstream(std::cout)
            << "net_get_id: Received " << bytes_transferred << " bytes"
            << std::endl;
        try {
            std::string seralizedData(_recv_buffer.data(), bytes_transferred);
            std::istringstream iss(seralizedData);
            boost::archive::binary_iarchive archive(iss);
            Utils::PlayerId tmp;
            archive >> tmp;
            _player_id = tmp.id;
            state = gameState::Playing;
        } catch (std::exception err) {
            std::cerr << "serialization exception: " << err.what();
            throw err;
        }
    } else {
        std::osyncstream(std::cout)
            << "Erreur dans net_get_id: " << error.message() << std::endl;
    }
}

void udp_client::fetch_player_id()
{
    std::chrono::system_clock::time_point timeout =
        std::chrono::system_clock::now();
    _socket.send_to(boost::asio::buffer({'\0'}, 1), _remote_point);
    boost::asio::ip::udp::endpoint test;
    _socket.async_receive_from(
        boost::asio::buffer(_recv_buffer), test,
        boost::bind(
            &udp_client::net_get_id, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    while (_player_id == -1) {
        if (std::chrono::system_clock::now() - timeout >
            std::chrono::seconds(10)) {
            std::cerr << "Failed to get player id from server\n";
            throw std::runtime_error("Failed to get player id from server");
        }
        _socket.send_to(boost::asio::buffer({'\0'}, 1), _remote_point);
        std::osyncstream(std::cout) << "looping " << std::endl;
        _svc.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::osyncstream(std::cout) << "player id: " << _player_id << "\n";
}

udp_client::udp_client(
    boost::asio::io_context &svc, const std::string &ip,
    const std::string &port, Registry &reg)
    : _socket(svc), timer(svc), _reg(reg), _svc(svc)
{
    _socket.open(udp::v4());
    udp::resolver resolver(svc);
    udp::resolver::query query(boost::asio::ip::udp::v4(), ip, port);
    udp::resolver::iterator iter = resolver.resolve(query);
    _remote_point = *iter;
    fetch_player_id();
    // std::thread fetch(&udp_client::fetch_player_id, this);
    // fetch.join();
    tick = std::thread(&udp_client::handle_tick, this);  // Timer thread
    sending = std::thread(&udp_client::send_user, this); // Snapshot thread
    receive = std::thread(&udp_client::start_receive, this);
    tick.detach();
    sending.detach();
    receive.detach();
    cmd.reset();
    // start_receive();
}

udp_client::~udp_client()
{
    _socket.close();
    tick.std::thread::~thread();
    sending.std::thread::~thread();
}

// int main(int ac, char **av)
// {
//     try {
//         boost::asio::io_context _svc;
//         std::string port = "5000";
//         std::string ip = "127.0.0.1";
//         if (ac == 3 && std::stoi(av[2])) {
//             ip = av[1];
//             port = av[2];
//         }
//         udp_client client(_svc, ip, port);
//         _svc.run();
//     } catch (std::exception &e) {
//         std::cerr << e.what() << std::endl;
//     }
// }

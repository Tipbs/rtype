#pragma once
#if defined(_WIN32)
#define NOGDI  // All GDI defines and routines
#define NOUSER // All USER defines and routines
#endif

#include <thread>
#if defined(_WIN32) // raylib uses these names as function parameters
#undef near
#undef far
#endif
// mandatory since raylib and windows.h are incompatible, thread include
// windows.h

#include <cstddef>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>
#include "../../shared/Registry.hpp"
#include "../../shared/UserCmd.hpp"

class udp_client {
  public:
    udp_client(boost::asio::io_context &svc, Registry &reg);
    ~udp_client();

    void connect(const std::string &ip, const std::string &port);

    void send();
    void start_receive();
    void handle_receive(const boost::system::error_code &, std::size_t);
    void handle_send(
        const boost::system::error_code &error, std::size_t bytes_transfered);
    void handle_tick();
    void send_user();

    Utils::PlayerId get_player_id() { return _player_id; }

  private:
    void fetch_player_id();
    void net_get_id(
        const boost::system::error_code &error, std::size_t bytes_transfered);
    Registry &_reg;
    std::vector<char> _recv_buffer = std::vector<char>(2048);
    Utils::PlayerId _player_id;
    boost::asio::ip::udp::socket _socket;
    boost::asio::ip::udp::endpoint _remote_point;
    boost::asio::io_context &_svc;
    boost::asio::deadline_timer timer;

    std::thread tick;
    std::thread sending;
    std::thread receive;
    std::mutex cmd_mutex;

    UserCmd cmd;
};

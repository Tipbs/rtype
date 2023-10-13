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
// mandatory since raylib and windows.h are incompatible, thread include windows.h

#include <cstddef>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>
#include "../../shared/UserCmd.hpp"
#include "../../shared/Registry.hpp"

class udp_client {
    public:
        udp_client(boost::asio::io_context &, const std::string &ip, const std::string &port, Registry &reg);
        ~udp_client();

        void send();
        void start_receive();
        void handle_receive(const boost::system::error_code &, std::size_t);
        void handle_send(const boost::system::error_code &error, std::size_t bytes_transfered);
        void handle_tick();
        void send_user();
    private:
        Registry &_reg;
        std::vector<char> _recv_buffer = std::vector<char>(512);
        boost::asio::ip::udp::socket _socket;
        boost::asio::ip::udp::endpoint _remote_point;
        boost::asio::io_context _svc;
        boost::asio::deadline_timer timer;

        std::thread tick;
        std::thread sending;
        std::mutex cmd_mutex;

        UserCmd cmd;
};

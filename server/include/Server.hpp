#include <map>
#include <thread>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/array.hpp>
#include <chrono>
#include "../../shared/UserCmd.hpp"

struct Message {
    int type;
};

class udp_server {
    public:
        udp_server(std::size_t port);
        ~udp_server();
        void handle_receive(const boost::system::error_code &, std::size_t);
        void handle_response();
        void handle_send(const boost::system::error_code &, std::size_t);
        void broadcast();
        void handle_broadcast(const boost::system::error_code &, std::size_t);
        void multiple_broadcast(std::vector<boost::asio::ip::udp::endpoint>, std::map<std::size_t ,std::vector<UserCmd>>);
        void add_buff_to_cmd();
    private:
        void start_receive();

        std::size_t _port;
        std::size_t id = 0;
        boost::asio::io_context _svc;
        boost::asio::ip::udp::socket _socket;
        boost::asio::ip::udp::endpoint _remote_point;
        std::vector<boost::asio::ip::udp::endpoint> clients;
        std::map<std::size_t, std::vector<UserCmd>> cmd;
        boost::array<char, 512> _recv_buffer;
};

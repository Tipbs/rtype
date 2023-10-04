#include <boost/archive/binary_iarchive.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/array.hpp>
#include <chrono>
#include <map>
#include "../../shared/UserCmd.hpp"

namespace boost {
#ifdef BOOST_NO_EXCEPTIONS
void throw_exception(std::exception const &e)
{
    throw e; // or whatever
};
void throw_exception(std::exception const &e, boost::source_location const &)
{
    throw e; // or whatever
};
#endif
} // namespace boost


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
        void deserialize(const std::size_t);
        void handle_tick(const boost::system::error_code &);
    private:
        void start_receive();

        std::size_t _port;
        std::size_t id = 0;
        boost::asio::io_context _svc;
        boost::asio::ip::udp::socket _socket;
        boost::asio::deadline_timer timer;
        boost::asio::ip::udp::endpoint _remote_point;
        std::vector<boost::asio::ip::udp::endpoint> clients;
        std::map<std::size_t, std::vector<UserCmd>> cmd;
        boost::array<char, 512> _recv_buffer;
};

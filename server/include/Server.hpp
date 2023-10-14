#include <chrono>
#include <cstdint>
#include <map>
#include <mutex>
#include <semaphore>
#include <thread>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/array.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
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
    void handle_tick();
    void handle_check(const boost::system::error_code &error);

    void broadcast();
    void handle_broadcast(const boost::system::error_code &, std::size_t);
    void multiple_broadcast(
        std::map<boost::asio::ip::udp::endpoint, boost::posix_time::ptime>,
        std::map<std::size_t, std::vector<UserCmd>>);
    void deserialize(const std::size_t);

    void start_check();
    void start_receive();

  private:
    std::size_t _port;

    boost::asio::io_context _svc;
    boost::asio::ip::udp::socket _socket;
    boost::asio::deadline_timer tick_timer;
    boost::asio::deadline_timer check_timer;
    boost::asio::ip::udp::endpoint _remote_point;
    boost::array<char, 512> _recv_buffer;

    std::map<boost::asio::ip::udp::endpoint, boost::posix_time::ptime> clients;
    std::map<std::size_t, std::vector<UserCmd>> cmd;

    std::thread tick;
    std::thread broadcasting;
    std::mutex cmd_mutex;
};

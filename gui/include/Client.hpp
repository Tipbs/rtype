#include <cstddef>
#include <thread>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>
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

class udp_client {
  public:
    udp_client(
        boost::asio::io_context &, const std::string &ip,
        const std::string &port);
    ~udp_client();

    void send();
    void start_receive();
    void handle_receive(const boost::system::error_code &, std::size_t);
    void handle_send(
        const boost::system::error_code &error, std::size_t bytes_transfered);
    void handle_tick();
    void send_user();

  private:
    boost::array<char, 512> _recv_buffer;
    boost::asio::ip::udp::socket _socket;
    boost::asio::ip::udp::endpoint _remote_point;
    boost::asio::io_context _svc;
    boost::asio::deadline_timer timer;

    std::thread tick;
    std::thread sending;
    std::mutex cmd_mutex;

    UserCmd cmd;
};

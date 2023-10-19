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

#include "../../shared/NetEnt.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/Systems.hpp"
#include "../../shared/UserCmd.hpp"
#include "../../shared/Utils.hpp"

struct Clients {
    std::size_t _id;
    bool isClientConnected;
    boost::posix_time::ptime _timer;
};

class udp_server {
  public:
    udp_server(std::size_t port);
    ~udp_server();

    void handle_receive(const boost::system::error_code &, std::size_t);
    void handle_response();
    void handle_tick();
    void handle_check(const boost::system::error_code &error);
    void handle_send(const boost::system::error_code &error, std::size_t);

    void start_snapshot();
    void send_playerId(std::size_t playerId, boost::asio::ip::udp::endpoint);
    void wait_for_connexion(std::size_t);
    void handle_broadcast(const boost::system::error_code &, std::size_t);
    void multiple_broadcast(
        std::map<boost::asio::ip::udp::endpoint, struct Clients>,
        std::vector<NetEnt>);
    void deserialize(const std::size_t);

    void start_threads();
    void start_check();
    void start_receive();
    void run_system();

  private:
    Registry reg;

    std::size_t _port;

    boost::asio::io_context _svc;
    boost::asio::ip::udp::socket _socket;
    boost::asio::deadline_timer tick_timer;
    boost::asio::deadline_timer check_timer;
    boost::asio::ip::udp::endpoint _remote_point;
    boost::array<char, 512> _recv_buffer;

    std::map<boost::asio::ip::udp::endpoint, struct Clients> clients;
    std::map<std::size_t, std::vector<UserCmd>> cmd;

    std::thread tick;
    std::thread broadcasting;
    std::mutex cmd_mutex;
};

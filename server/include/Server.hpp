#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/array.hpp>

struct pos {
    int x;
    int y;
};

struct test_struct {
    int type;
    int id;
    struct pos pos;
};

struct Message {
    int type;
};

class udp_server {
    public:
        udp_server(boost::asio::io_context &_svc, std::size_t port);
        ~udp_server();
        void handle_receive(const boost::system::error_code &, std::size_t);
        void handle_response();
        void handle_send(const boost::system::error_code &, std::size_t);
    private:
        void start_receive();
        boost::asio::ip::udp::socket _socket;
        boost::asio::ip::udp::endpoint _remote_point;
        boost::array<char, 512> _recv_buffer;
};

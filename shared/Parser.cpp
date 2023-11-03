#include "Parser.hpp"
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/json_parser.hpp>

Parser::Parser(Registry &registry) : _factory(Factory(registry)) {}

Parser::Parser(Registry &registry, std::string file):
    _factory(Factory(registry))
{
    /*for (ptree::value_type &entity : _root.get_child("players")) {
        int id = entity.second.get<int>("id");
        Position pos = { entity.second.get<double>("pos.x"), entity.second.get<double>("pos.y")};
        _factory.create_player(id, pos);
    }*/
}

void Parser::open_file(std::string file)
{
    _root.clear();
    pt::read_json(file, _root);
}

const Entity Parser::create_player(std::size_t id)
{
    try {
        for (ptree::value_type &entity : _root.get_child("players")) {
            size_t _id = entity.second.get<int>("id");
            if (_id == id) {
                Position pos = {
                    entity.second.get<double>("pos.x"),
                    entity.second.get<double>("pos.y")};
                Entity player = _factory.create_player(pos, id);
                _factory.create_weapon(player);
                return player;
            }
        }
    } catch (boost::property_tree::ptree_error &e) {
        std::cout << e.what();
    }
    return -1;
}

Utils::Vec2 Parser::get_player_pos(std::size_t id)
{
    try {
        for (ptree::value_type &entity : _root.get_child("players")) {
            size_t _id = entity.second.get<int>("id");
            if (_id == id) {
                Utils::Vec2 pos;
                pos.x = entity.second.get<double>("pos.x");
                pos.y = entity.second.get<double>("pos.y");
                return pos;
            }
        }
    } catch (boost::property_tree::ptree_error &e) {
        std::cout << e.what();
    }
    return {-1, -1};
}

int Parser::get_enemy_count()
{
    try {
        for (ptree::value_type &entity : _root.get_child("enemies")) {
            std::string count = entity.second.get_value("count");
            std::cout << "count is = " << count << std::endl;
            return std::stoi(count);
        }
    } catch (boost::property_tree::ptree_error &e) {
        std::cout << e.what();
    }
    return 0;
}

int Parser::get_boss_count()
{
    try {
        for (ptree::value_type &entity : _root.get_child("boss")) {
            std::string count = entity.second.get_value("count");
            std::cout << "count is = " << count << std::endl;
            return std::stoi(count);
        }
    } catch (boost::property_tree::ptree_error &e) {
        std::cout << e.what();
    }
    return 0;
}

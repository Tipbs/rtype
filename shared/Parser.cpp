#include "Parser.hpp"
#include "Utils.hpp"

Parser::Parser(Registry &r) : _factory(Factory(r)) {}

void Parser::open_file(std::string file)
{
    _root.clear();
    pt::read_json(file, _root);
}

Parser::Parser(Registry &registry, std::string file):
    _factory(Factory(registry))
{
    pt::read_json(file, _root);
    /*for (ptree::value_type &entity : _root.get_child("players")) {
        int id = entity.second.get<int>("id");
        Position pos = { entity.second.get<double>("pos.x"), entity.second.get<double>("pos.y")};
        _factory.create_player(id, pos);
    }*/
}

const Entity Parser::create_player(std::size_t id)
{
    for (ptree::value_type &entity : _root.get_child("players")) {
        int _id = entity.second.get<int>("id");
        if (_id == id) {
            Position pos = { entity.second.get<double>("pos.x"),
                entity.second.get<double>("pos.y")};
            Entity player = _factory.create_player(id, pos);
            _factory.create_weapon(player);
            return player;
        }
    }
    return -1;
}

Utils::Vec2 Parser::get_player_pos(std::size_t id)
{
    for (ptree::value_type &entity : _root.get_child("players")) {
        int _id = entity.second.get<int>("id");
        if (_id == id) {
            Utils::Vec2 pos;
            pos.x = entity.second.get<double>("pos.x");
            pos.y = entity.second.get<double>("pos.y");
            return pos;
        }
    }
    return {-1, -1};
}

int Parser::get_enemy_count()
{
    for (ptree::value_type &entity : _root.get_child("enemies")) {
        std::string count = entity.second.get_value("count");
        std::cout << "count is = " << count << std::endl;
        return std::stoi(count);
    }
    return 0;
}

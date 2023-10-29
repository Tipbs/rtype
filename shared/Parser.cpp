#include "Parser.hpp"


Parser::Parser(Registry &registry, std::string file):
    _factory(Factory(registry))
{
    pt::read_json(file, _root);
    for (ptree::value_type &entity : _root.get_child("players")) {
        int id = entity.second.get<int>("id");
        Position pos = { entity.second.get<double>("pos.x"), entity.second.get<double>("pos.y")};
        _factory.create_player(id, pos);
    }
}

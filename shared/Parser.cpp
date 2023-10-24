#include "Parser.hpp"


Parser::Parser(Registry &registry, std::string file):
    _root(ptree::read_json(file, _root)), _factory(Factory(registry))
{
    for (ptree::value_type &entity : _root.get_child("players")) {
        int id = entity.id;
        Utils::Vec2 pos = entity.pos;
        int type = entity.type;
        _factory.create(type, id, pos, type);
    }
}

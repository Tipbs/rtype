#pragma once

#include "NetEnt.hpp"
#include <string>
#include "Factory.hpp"
#include "Utils.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

class Parser {
    public:
        Parser(Registry &registry, std::string file);
        Parser(Registry &registry);
        Utils::Vec2 get_player_pos(std::size_t id);
        void open_file(std::string file);
        const Entity create_player(std::size_t id);
        int get_enemy_count();
        int get_boss_count();
    
    public:
        using ptree = pt::ptree;
        using entity = std::vector<std::tuple<int, Utils::Vec2, uint8_t>>;
    
    private:
        ptree _root;
        Factory _factory;
};

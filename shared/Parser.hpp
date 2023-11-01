#pragma once

#include "../../shared/NetEnt.hpp"
#include <string>
#include "Factory.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

class Parser {
    public:
        Parser(Registry &registry, std::string file);
    
    public:
        using ptree = pt::ptree;
        using entity = std::vector<std::tuple<int, Utils::Vec2, uint8_t>>;
    
    private:
        ptree _root;
        Factory _factory;
};

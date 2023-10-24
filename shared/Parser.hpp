#pragma once

#include "../../shared/NetEnt.hpp"
#include <string>
#include "Factory.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class Parser {
    public:
        Parser(Registry &_registry, std::string file);
    
    public:
        using ptree = boost::property_tree::ptree;
        using entity = std::vector<int, Utils::Vec2, uint8_t>;
    
    private:
        ptree _root;
        Factory &_factory;
};

#pragma once
#include <vector>
#include "../../shared/Systems.hpp"
#include "GraphicComponents.hpp"
#include "../../shared/Sparse_array.hpp"
#include "../../shared/Bundle.hpp"

class Players_info {
    private:
        std::vector<size_t> players_id;
        size_t current_player;
    public:
        Players_info() {current_player = -1; players_id = {};};
        Players_info(size_t id): current_player(id) {players_id = {};};
        void add_current_player(size_t id);
        void add_player(size_t id);
        size_t get_current();
        std::vector<size_t> get_all_players();
};


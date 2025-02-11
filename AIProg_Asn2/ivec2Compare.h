#pragma once
#include <glm/glm.hpp>

struct ivec2Compare {
    bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
        if (a.x < b.x) {
            return true;
        }
        if (a.x > b.x) {
            return false;
        }
        return a.y < b.y;
    }
};
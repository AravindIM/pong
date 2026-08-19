#pragma once
#include <random>
#include <utility>

inline std::mt19937& rng() {
    static std::mt19937 gen{ std::random_device{}() };
    return gen;
}

template <typename Dist, typename... Args>
typename Dist::result_type random(Args&&... args) {
    Dist dist(std::forward<Args>(args)...);
    return dist(rng());
}

inline int getRandomSign() { return random<std::bernoulli_distribution>(0.5) ? 1 : -1; }

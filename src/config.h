#pragma once

#include <toml++/toml.h>

namespace blood
{
struct HitBurstConfig
{
    bool enabled      = true;
    bool enable_block = false;
    bool enable_dead  = true;

    float   num_per_damage = 0.1f;
    float   min_damage     = 10.f;
    uint8_t max_num        = 20;

    float speed_spread = 1000.f;
    float angle_spread = 15.f;

    float scale = 0.2;

    static void from_toml(const toml::table& tbl, HitBurstConfig& config);
};

struct Config
{
    float effect_range = 4096.f;

    HitBurstConfig hit_burst   = {};
    bool           decap_spurt = true;

    StrMap<RE::BGSProjectile*> proj_keyword_map = {};

    static Config* getSingleton()
    {
        static Config config;
        return std::addressof(config);
    }

    static void from_toml(const toml::table& tbl, Config& config);
    void        loadFile();
};
} // namespace blood
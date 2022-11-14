#include "config.h"

namespace blood
{
void HitBurstConfig::from_toml(const toml::table& tbl, HitBurstConfig& config)
{
    config = {
        .enabled        = tbl["enabled"].value_or(true),
        .enable_block   = tbl["enable_block"].value_or(false),
        .enable_dead    = tbl["enable_dead"].value_or(true),
        .num_per_damage = tbl["num_per_damage"].value_or(0.1f),
        .min_damage     = tbl["min_damage"].value_or(0.1f),
        .max_num        = tbl["max_num"].value_or<uint8_t>(20),
        .speed_spread   = tbl["speed_spread"].value_or(1000.f),
        .angle_spread   = tbl["angle_spread"].value_or(15.f),
        .scale          = tbl["scale"].value_or(1.f),
    };
}

void Config::from_toml(const toml::table& tbl, Config& config)
{
    config.effect_range = tbl["effect_range"].value_or(4096.f);

    HitBurstConfig::from_toml(*tbl["hit_burst"].as_table(), config.hit_burst);

    config.proj_keyword_map.clear();
    for (auto& [k, v] : *tbl["projectile_keywords"].as_table())
    {
        auto proj = RE::TESForm::LookupByEditorID<RE::BGSProjectile>(v.value_or(""));
        if (proj)
            config.proj_keyword_map.emplace(k.str(), proj);
        else
            logger::warn("Cannot find projectile {}", v.value_or(""));
    }
}

void Config::loadFile()
{
    try
    {
        auto tbl = toml::parse_file("data/skse/plugins/BloodProjectile.toml");
        Config::from_toml(tbl, *this);
        logger::info("Configuration loaded.");
    }
    catch (const toml::parse_error& err)
    {
        logger::warn("Failed to parse configuration file. Using default settings.\nError: {}", err.description());
    }
}
} // namespace blood
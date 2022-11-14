#pragma once

namespace blood
{
inline RE::BGSProjectile* getProjectileType(RE::Actor* actor)
{
    for (auto& [keyword, proj] : Config::getSingleton()->proj_keyword_map)
        if (actor->HasKeywordString(keyword))
            return proj;
    return nullptr;
}

inline RE::Projectile::LaunchData getDefaultLaunchData()
{
    RE::Projectile::LaunchData launch_data;
    launch_data.shooter               = nullptr;
    launch_data.combatController      = nullptr;
    launch_data.weaponSource          = nullptr;
    launch_data.ammoSource            = nullptr;
    launch_data.unk50                 = nullptr;
    launch_data.desiredTarget         = nullptr;
    launch_data.unk60                 = 0.f;
    launch_data.unk64                 = 0.f;
    launch_data.spell                 = nullptr;
    launch_data.enchantItem           = nullptr;
    launch_data.poison                = nullptr;
    launch_data.area                  = 0;
    launch_data.power                 = 1.f;
    launch_data.scale                 = 1.f;
    launch_data.alwaysHit             = false;
    launch_data.noDamageOutsideCombat = true;
    launch_data.autoAim               = false;
    launch_data.unk9F                 = false;
    launch_data.useOrigin             = true;
    launch_data.deferInitialization   = false;
    launch_data.forceConeOfFire       = false;
    return launch_data;
}
} // namespace blood
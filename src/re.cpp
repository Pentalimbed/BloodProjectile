#include "re.h"

#include "config.h"
#include "emitter.h"
#include "projectile.h"

RE::Projectile::LaunchData::~LaunchData() {}

namespace blood
{
void ProcessHitHook::thunk(RE::Actor* a_victim, RE::HitData& a_hitData)
{
    if (!a_victim)
        return;

    auto config = Config::getSingleton();

    if (config->hit_burst.enabled)
        weapBurstEmit(a_victim, a_hitData);

    // ProjectileSpawner::getSingleton()->addSpawner([=](float time) -> SpawnerReturn { return spawnBleedProjectile(a_victim, a_hitData, time); });

    func(a_victim, a_hitData);
}

void UpdateHook::thunk(RE::Main* a_this, float a2)
{
    func(a_this, a2);
    EmitterManager::getSingleton()->update();
}
} // namespace blood
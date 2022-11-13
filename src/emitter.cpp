#include "emitter.h"

#include "config.h"
#include "projectile.h"

#include <effolkronium/random.hpp>

inline RE::NiPoint3 randVecInCone(RE::NiPoint3 axis, float half_angle)
{
    RE::NiPoint3 u   = {axis.y, -axis.x, 0};
    RE::NiPoint3 v   = axis.Cross(u);
    float        phi = effolkronium::random_static::get(0.L, 2 * std::_Pi);
    float        z   = effolkronium::random_static::get(cos(half_angle), 1.f);
    return (u * cos(phi) + v * sin(phi)) * std::sqrt(1 - z * z) + axis * z;
}

namespace blood
{


void weapBurstEmit(RE::Actor* victim, RE::HitData& hitdata)
{
    auto  config       = Config::getSingleton();
    auto& burst_config = config->hit_burst;

    if (victim->IsGhost())
        return;
    if (victim->IsDead() && !burst_config.enable_dead)
        return;
    if (victim->IsBlocking() && (hitdata.percentBlocked > 1.f) && !burst_config.enable_block)
        return;
    if (hitdata.totalDamage < burst_config.min_damage)
        return;

    auto cell = victim->parentCell;
    if (!cell)
        return;
    auto proj_type = getProjectileType(victim);
    if (!proj_type)
        return;
    auto cam = RE::PlayerCamera::GetSingleton();
    if (!cam)
        return;
    if (hitdata.hitPosition.GetDistance(cam->pos) > config->effect_range)
        return;

    auto launch_data           = getDefaultLaunchData();
    launch_data.origin         = hitdata.hitPosition;
    launch_data.projectileBase = proj_type;
    launch_data.parentCell     = cell;
    launch_data.scale          = burst_config.scale;

    uint8_t proj_num     = std::clamp(burst_config.num_per_damage * hitdata.totalDamage, 0.f, (float)burst_config.max_num);
    float   angle_spread = burst_config.angle_spread * std::_Pi / 180.f;
    for (int i = 0; i < proj_num; ++i)
    {
        auto dir = hitdata.hitDirection;
        dir.Unitize();
        dir        = randVecInCone(dir, angle_spread / 2);
        auto rot_x = atan2(-dir.z, std::sqrtf(dir.x * dir.x + dir.y * dir.y));
        auto rot_z = atan2(dir.x, dir.y);

        launch_data.angleX = rot_x + effolkronium::random_static::get(-angle_spread / 2, angle_spread / 2);
        launch_data.angleZ = rot_z + effolkronium::random_static::get(-angle_spread / 2, angle_spread / 2);

        RE::BSPointerHandle<RE::Projectile> handle;
        RE::Projectile::Launch(&handle, launch_data);

        if (handle && handle.get())
        {
            auto& runtime_data = handle.get()->GetProjectileRuntimeData();

            auto linear_velo = runtime_data.linearVelocity;
            linear_velo.Unitize();
            runtime_data.linearVelocity += linear_velo * effolkronium::random_static::get(-burst_config.speed_spread / 2, burst_config.speed_spread / 2);
        }
    }
}
} // namespace blood
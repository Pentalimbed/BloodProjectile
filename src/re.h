#pragma once

namespace blood
{
/* ---------------- HOOKS ---------------- */
struct ProcessHitHook
{
    static void                                    thunk(RE::Actor* a_victim, RE::HitData& a_hitData);
    static inline REL::Relocation<decltype(thunk)> func;

    static constexpr auto id     = RELOCATION_ID(37673, 38627);
    static constexpr auto offset = REL::VariantOffset(0x3c0, 0x4a8, 0x0); // VR Unknown
};

struct UpdateHook
{
    static void                                    thunk(RE::Main* a_this, float a2);
    static inline REL::Relocation<decltype(thunk)> func;

    static constexpr auto id     = RELOCATION_ID(35551, 36544);
    static constexpr auto offset = REL::VariantOffset(0x11F, 0x160, 0x0); // VR Unknown
};

} // namespace blood
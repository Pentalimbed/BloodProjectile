#pragma once

namespace blood
{
void weapBurstEmit(RE::Actor* a_victim, RE::HitData& a_hitData);

struct Emitter
{
    float        time                   = 0.f;
    virtual bool isValid()              = 0;
    virtual void emit(float delta_time) = 0;
};

// struct WeapTrailEmitter : Emitter
// {
//     float      duration     = 1.f;
//     uint8_t    proj_per_sec = 10;
//     RE::Actor* actor        = nullptr;

//     inline bool isValid() override { return (time < duration) && actor; }
//     void        emit(float delta_time) override;
// };

class EmitterManager
{
    std::vector<std::shared_ptr<Emitter>> active_emitters = {};

public:
    static EmitterManager* getSingleton()
    {
        static EmitterManager manager;
        return std::addressof(manager);
    }

    inline void addEmitter(std::shared_ptr<Emitter> emitter) { active_emitters.push_back(emitter); }
    inline void update()
    {
        auto delta_time = *REL::Relocation<float*>{RELOCATION_ID(523661, 410200)};
        std::erase_if(active_emitters,
                      [=](auto emitter) {
                          if (!emitter->isValid())
                              return true;
                          emitter->time += delta_time;
                          emitter->emit(delta_time);
                          return false;
                      });
    }
};
} // namespace blood

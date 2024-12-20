#pragma once

#include <Hazel.h>

struct ParticleProps
{
    glm::vec2 Position;
    glm::vec2 Velocity, VelocityVariation;
    glm::vec4 ColorBegin, ColorEnd;
    float SizeBegin, SizeEnd, SizeVariation;
    float MinLifeTime = 5.0f;
    float MaxLifeTime = 15.0f;
};

class ParticleSystem
{
public:
    explicit ParticleSystem(uint32_t maxParticles = 100000);

    void OnUpdate(Hazel::Timestep ts);
    void OnRender(const Hazel::OrthographicCamera& camera);

    void Emit(const ParticleProps& particleProps);
private:
    struct Particle
    {
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec4 ColorBegin, ColorEnd;
        float Rotation = 0.0f;
        float SizeBegin, SizeEnd;

        float LifeTime = 1.0f;
        float LifeRemaining = 0.0f;

        bool Active = false;
    };
    std::vector<Particle> m_ParticlePool;
    uint32_t m_PoolIndex = 999;
};
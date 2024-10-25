#include "Particle.hlsli"

struct TransFormationMatrix
{
	float32_t4x4 WVP;
	float32_t4x4 World;
};
struct ParticleForGPU{
    float32_t4x4 WVP;
    float32_t4x4 World;
	float32_t4 color;
};
StructuredBuffer<ParticleForGPU> gParticle : register(t0);

struct VertexShaderInput
{
	float32_t4 position : POSITION0;
	float32_t2 texccord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
	VertexShaderOutput output;
    output.position = mul(input.position, gParticle[instanceId].WVP);
	output.texcoord = input.texccord;
    output.color = gParticle[instanceId].color;
	return output;
}
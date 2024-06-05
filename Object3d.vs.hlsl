#include "object2d.hlsli"

struct TransFormationMatrix
{
    float32_t4x4 WVP;
};
ConstantBuffer<TransFormationMatrix> gTransformationMatrix : register(b0);



struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texccord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gTransformationMatrix.WVP);
    output.texcoord = input.texccord;
    return output;
}
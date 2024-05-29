
struct Material
{
    float32_t4 color;
};
ConstantBuffer<Material> gMaterial:register(b0);
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main()
{
    PixelShaderOutput outout;
    outout.color = gMaterial.color;
    return outout;
}
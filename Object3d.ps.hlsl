
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main()
{
    PixelShaderOutput outout;
    outout.color = float32_t4(1.0, 1.0, 1.0, 1.0);
    return outout;
}
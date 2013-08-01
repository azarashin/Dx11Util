cbuffer EffectCommetMatInfo : register(b0) {
	matrix mat; 
	float alpha; 
}; 

struct VS_INPUT {
    float3 Pos  : POSITION; 
    float3 Normal  : NORMAL; 
    float2 Tex  : TEXTURE;  
};

struct PS_INPUT {
    float4 Pos  : SV_POSITION; // 頂点座標(透視座標系)
    float2 Tex  : TEXTURE;  
};


PS_INPUT VS(VS_INPUT input) {
    PS_INPUT output;
	float4 pos, opos; 
	pos.xyz = input.Pos; 
	pos.w = 1.0; 
    opos = mul(pos, mat); 
	output.Pos = opos; 
    output.Tex = input.Tex;

    return output;
}

// ピクセル シェーダの関数
float4 PS(PS_INPUT input) : SV_TARGET
{
	float4 texCol; 
	float dx = 2.0 * (0.5 - input.Tex.x); 
	float dy = 2.0 * (0.5 - input.Tex.y); 
	float d2 = dx * dx + dy * dy; 
	float c = max(0.0, 1.0-d2);
	texCol.rgb = float3(c, c, c); 
	texCol.a = alpha; 
	return texCol; 
}

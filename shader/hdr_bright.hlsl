cbuffer EffectGUIStandardInfo : register(b0) {
	float4 br_weight; 
	float4 br_offset; 
}; 

struct VS_INPUT {
    float3 Pos  : POSITION; 
    float3 Normal  : NORMAL; 
    float3 Tangent  : TANGENT; 
    float3 Binormal  : BINORMAL; 
    float4 Weight  : WEIGHT; 
	int4 Windex  : WEIGHT_INDEX; 
	float4 Color   : COLOR; 
    float2 Tex  : TEXTURE;  
};

struct PS_INPUT {
    float4 Pos  : SV_POSITION; // 頂点座標(透視座標系)
    float2 Tex  : TEXTURE;  
};

Texture2D myTex2D[2];   // テクスチャ

// サンプラ
SamplerState texSampler : register(s0);

PS_INPUT VS(VS_INPUT input) {
    PS_INPUT output;
    output.Pos = float4(
		input.Pos.x * 2.0 - 1.0, 
		input.Pos.y * 2.0 - 1.0, 
		input.Pos.z, 
		1.0); 
    output.Tex = input.Tex;
	output.Tex.x = 1.0 - output.Tex.x; 
    return output;
}

// ピクセル シェーダの関数
float4 PS(PS_INPUT input) : SV_TARGET
{
    float4 texCol = myTex2D[0].Sample(texSampler, input.Tex);         // テクセル読み込み
    float4 texCol2 = myTex2D[1].Sample(texSampler, float2(0.5, 0.5));
//	float BrOffset = max(length(texCol2.xyz) - 0.5, 0.0) + 1.0;
	float lngt = length(texCol2.xyz); 
	texCol.xyz = (1.0 - exp((-texCol.xyz) / lngt)); 
	return max((texCol - br_offset) * br_weight, float4(0.0, 0.0, 0.0, 1.0)); 
}

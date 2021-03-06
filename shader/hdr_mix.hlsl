cbuffer EffectGUIStandardInfo : register(b0) {
	float4 offset; 
	float4 weight; 
	int bright_num; 
	int weight_enable; 
	float2 dummy; 
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

Texture2D myTex2D[8];   // 0: base, 1: hdr_src, 2 ~ bright_num: brightness; 

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
	int i; 
	float lngt = 0.0; 
	float wgt = 1.0; 
	float3 col; 
	float4 texCol = float4(0.0, 0.0, 0.0, 1.0); 
	float4 texCol2 = myTex2D[1].Sample(texSampler, float2(0.5, 0.5));
	texCol = myTex2D[0].Sample(texSampler, input.Tex); 
	for(i=2;i<bright_num;i++) {
		texCol.xyz += myTex2D[i].Sample(texSampler, input.Tex).xyz; 
	}
	if(weight_enable == 1) {
		lngt = length(texCol2.xyz); 
		texCol.xyz = (1.0 - exp((-texCol.xyz) / lngt)); 
	}
	return texCol; 
}

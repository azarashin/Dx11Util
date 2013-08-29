cbuffer EffectGUIStandardInfo : register(b0) {
	float x_pos, y_pos, width, height; 
	float trans; 
	float inv; 
	float dummy1, dummy2; 
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

Texture2D myTex2D;   // テクスチャ

// サンプラ
SamplerState texSampler : register(s0);

PS_INPUT VS(VS_INPUT input) {
    PS_INPUT output;
    output.Pos = float4(
		(input.Pos.x * width + x_pos) * 2.0 - 1.0, 
		(input.Pos.y * height + y_pos) * 2.0 - 1.0, 
		input.Pos.z, 
		1.0); 
    output.Tex = input.Tex;
	output.Tex.x = 1.0 - output.Tex.x; 

    return output;
}

// ピクセル シェーダの関数
float4 PS(PS_INPUT input) : SV_TARGET
{
	float3 colmax = float3(1.0, 1.0, 1.0); 
    float4 texCol = myTex2D.Sample(texSampler, input.Tex);         // テクセル読み込み
	if(texCol.a == 0.0) {
		discard; 
	}
	texCol.rgb = texCol.rgb * (1.0 - inv) + (colmax - texCol.rgb) * inv; 
	texCol.a = texCol.a * trans; 
	return texCol; 
}

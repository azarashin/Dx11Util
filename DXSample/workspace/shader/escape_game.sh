cbuffer EffectEscapeGameInfo : register(b0) {
	float left_upper_small; 
	float right_upper_small; 
	float left_lower_small; 
	float right_lower_small; 
	float left_upper_middle; 
	float right_upper_middle; 
	float left_lower_middle; 
	float right_lower_middle; 
	float wave; 
	float trans; 
	float dummy2, dummy3; 
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

Texture2D myTex2D;   // テクスチャ

// サンプラ
SamplerState texSampler : register(s0);

PS_INPUT VS(VS_INPUT input) {
    PS_INPUT output;
    output.Pos = float4(
		(input.Pos.x * 1.0 + 0.0) * 2.0 - 1.0, 
		-(input.Pos.y * 1.0 + 0.0) * 2.0 + 1.0, 
		input.Pos.z, 
		1.0); 
    output.Tex = input.Tex;

    return output;
}

// ピクセル シェーダの関数
float4 PS(PS_INPUT input) : SV_TARGET
{
	float3 colmax = float3(1.0, 1.0, 1.0); 
	float r, b; 
	float2 tex2; 
	float d; 

	r =   clamp((0.5 - abs( 0.75 - input.Tex.x) - abs( 0.75 - input.Tex.y)) * right_lower_middle, 0.0, 1.0)
		+ clamp((0.5 - abs( 0.25 - input.Tex.x) - abs( 0.75 - input.Tex.y)) * left_lower_middle, 0.0, 1.0)
		+ clamp((0.5 - abs( 0.75 - input.Tex.x) - abs( 0.25 - input.Tex.y)) * right_upper_middle, 0.0, 1.0)
		+ clamp((0.5 - abs( 0.25 - input.Tex.x) - abs( 0.25 - input.Tex.y)) * left_upper_middle, 0.0, 1.0); 

	b =   clamp((0.5 - abs( 0.75 - input.Tex.x) - abs( 0.75 - input.Tex.y)) * right_lower_small, 0.0, 1.0)
		+ clamp((0.5 - abs( 0.25 - input.Tex.x) - abs( 0.75 - input.Tex.y)) * left_lower_small, 0.0, 1.0)
		+ clamp((0.5 - abs( 0.75 - input.Tex.x) - abs( 0.25 - input.Tex.y)) * right_upper_small, 0.0, 1.0)
		+ clamp((0.5 - abs( 0.25 - input.Tex.x) - abs( 0.25 - input.Tex.y)) * left_upper_small, 0.0, 1.0); 

	tex2 = input.Tex; 
	d = sqrt((0.5 - input.Tex.x) * (0.5 - input.Tex.x) + (0.5 - input.Tex.y) * (0.5 - input.Tex.y)); 
	tex2.x += wave * sin(d * 50.0 + wave * 50.0) * 0.01; 
	tex2.y += wave * cos(d * 50.0 + wave * 50.0) * 0.01; 
    float4 texCol = myTex2D.Sample(texSampler, tex2);         // テクセル読み込み
	if(texCol.a == 0.0) {
		discard; 
	}
//	texCol.rgb = texCol.rgb; 

	texCol.r += r; 
	texCol.b += b; 

	texCol.a = texCol.a * trans; 
	return texCol; 
}

cbuffer cbNeverChanges : register(b0) { // 常にスロット「0」を使う
	float trans; 
	float inv; 
	float dummy1, dummy2; 
	float4x4  World;
    float4x4  View;
    float4x4  Projection;
};

struct VS_INPUT {
    float3 Pos  : POSITION; 
    float3 Normal  : NORMAL; 
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


// 頂点シェーダの関数
PS_INPUT VS(VS_INPUT input) {
    PS_INPUT output;
    // 頂点座標　モデル座標系→ビュー座標系
    float4 pos4 = float4(input.Pos, 1.0);
	/*
    output.Pos = mul(pos4, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
	*/

    output.Pos = mul(World, pos4);
    output.Pos = mul(View, output.Pos);
    output.Pos = mul(Projection, output.Pos);

    // 頂点色
//    output.Col = float4(input.Col, 1.0);

    output.Tex = input.Tex;
    // 出力
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

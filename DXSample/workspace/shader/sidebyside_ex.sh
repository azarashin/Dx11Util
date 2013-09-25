cbuffer EffectGUIStandardInfo : register(b0) {
    float2 LensCenter;
    float2 ScreenCenter;
    float2 Scale;
    float2 ScaleIn;
    float4 HmdWarpParam;
	float4 ChromAbParam; 
	float left, dummy1, dummy2, dummy3; 
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
SamplerState texSampler[6] : register(s0);

PS_INPUT VS(VS_INPUT input) {
    PS_INPUT output;
    output.Pos = float4(
		input.Pos.x + left-0.5, 
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
//    float4 texCol = myTex2D.Sample(texSampler, input.Tex);         // テクセル読み込み
//	if(texCol.a == 0.0) {
//		discard; 
//	}
//	return texCol; 


    float2 theta = (input.Tex - LensCenter) * ScaleIn; // Scales to [-1, 1]
    float  rSq = theta.x * theta.x + theta.y * theta.y;
    float2 theta1 = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq + HmdWarpParam.z * rSq * rSq + HmdWarpParam.w * rSq * rSq * rSq);
	float2 tc = LensCenter + Scale * theta1; 

	if((tc.x < 0.0) || (tc.x > 1.0) || (tc.y < 0.0) || (tc.y > 1.0)) {
		discard; 
	}
//	return myTex2D.Sample(texSampler, tc); 


    // Detect whether blue texture coordinates are out of range since these will scaled out the furthest.
    float2 thetaBlue = theta1 * (ChromAbParam.z + ChromAbParam.w * rSq);
    float2 tcBlue = LensCenter + Scale * thetaBlue;
	if((tcBlue.x < 0.0) || (tcBlue.x > 1.0) || (tcBlue.y < 0.0) || (tcBlue.y > 1.0)) {
		discard; 
	}

    // Now do blue texture lookup.
    float  blue = myTex2D.Sample(texSampler[0], tcBlue).b;

    // Do green lookup (no scaling).
    float2 tcGreen = LensCenter + Scale * theta1;
    float4 greenColor = myTex2D.Sample(texSampler[0], tcGreen);
    float  green = greenColor.g;
    float  alpha = greenColor.a;

    // Do red scale and lookup.
    float2 thetaRed = theta1 * (ChromAbParam.x + ChromAbParam.y * rSq);
    float2 tcRed = LensCenter + Scale * thetaRed;
	if((tcRed.x < 0.0) || (tcRed.x > 1.0) || (tcRed.y < 0.0) || (tcRed.y > 1.0)) {
		discard; 
	}
    float  red = myTex2D.Sample(texSampler[0], tcRed).r;

    return float4(red, green, blue, alpha );

}

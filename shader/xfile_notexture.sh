cbuffer cbNeverChanges : register(b0) { // ��ɃX���b�g�u0�v���g��
	float4x4  World;
    float4x4  View;
    float4x4  Projection;
};

cbuffer cbSubMesh : register(b2) { // ��ɃX���b�g�u0�v���g��
	float4x4  bind_pose;
	float4 diffuse; // r, g, b, a
	float3 specular; // r, g, b
	float3 emmisive; // r, g, b
	float power; 
	int tex_enable; 
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
    float4 Pos  : SV_POSITION; // ���_���W(�������W�n)
    float3 ViewPos  : POSITION; // ���_���W(�r���[���W�n)
    float3 Normal  : NORMAL; 
	float4 Color   : COLOR; 
    float2 Tex  : TEXTURE;  
};

Texture2D myTex2D;   // �e�N�X�`��
// �T���v��
SamplerState texSampler : register(s0);


// ���_�V�F�[�_�̊֐�
PS_INPUT VS(VS_INPUT input) {
    PS_INPUT output;
    // ���_���W�@���f�����W�n���r���[���W�n
    float4 pos4 = float4(input.Pos, 1.0);
    float4 nor4 = float4(input.Normal, 0.0);

    output.Pos = mul(bind_pose, pos4);
    output.Pos = mul(World, output.Pos);
    output.Pos = mul(View, output.Pos);
	output.ViewPos = output.Pos.xyz; 
    output.Pos = mul(Projection, output.Pos);

	// Normal
    nor4 = mul(World, nor4);
    nor4 = mul(View, nor4);
	output.Normal = nor4.xyz; 


    // ���_�F
    output.Color = input.Color;

    output.Tex = input.Tex;
    // �o��
    return output;
}


// �s�N�Z�� �V�F�[�_�̊֐�
float4 PS(PS_INPUT input) : SV_TARGET
{
/*
	float3 colmax = float3(1.0, 1.0, 1.0); 
    float4 texCol = myTex2D.Sample(texSampler, input.Tex);         // �e�N�Z���ǂݍ���
	if(texCol.a == 0.0) {
		discard; 
	}
	texCol.rgb = texCol.rgb * (1.0 - inv) + (colmax - texCol.rgb) * inv; 
	texCol.a = texCol.a * trans; 
	return texCol; 
	*/

//	float4 ret = float4(1.0, 1.0, 1.0, 1.0); 
	float4 ret = diffuse; 
	return ret; 
}

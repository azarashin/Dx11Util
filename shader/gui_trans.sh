cbuffer cbNeverChanges : register(b0) { // ��ɃX���b�g�u0�v���g��
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
    float4 Pos  : SV_POSITION; // ���_���W(�������W�n)
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
	/*
    output.Pos = mul(pos4, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
	*/

    output.Pos = mul(World, pos4);
    output.Pos = mul(View, output.Pos);
    output.Pos = mul(Projection, output.Pos);

    // ���_�F
//    output.Col = float4(input.Col, 1.0);

    output.Tex = input.Tex;
    // �o��
    return output;
}


// �s�N�Z�� �V�F�[�_�̊֐�
float4 PS(PS_INPUT input) : SV_TARGET
{
	float3 colmax = float3(1.0, 1.0, 1.0); 
    float4 texCol = myTex2D.Sample(texSampler, input.Tex);         // �e�N�Z���ǂݍ���
	if(texCol.a == 0.0) {
		discard; 
	}
	texCol.rgb = texCol.rgb * (1.0 - inv) + (colmax - texCol.rgb) * inv; 
	texCol.a = texCol.a * trans; 
	return texCol; 
}

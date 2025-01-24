float4x4 gWorld;
float4x4 gView;
float4x4 gProj;

float4 gLightDirection  = float4(0, -1, 0, 0);
float4 gLightColor      = float4(1, 1, 1, 1);
float4 gAmbientColor    = float4(0.2, 0.2, 0.2, 1.0);

float4 gMaterialColor   = float4(1, 1, 1, 1);

bool gUseEmission = false;
float4 gMissionColor = float4(1, 1, 1, 1);

texture gDiffuseMap;
sampler2D DiffuseMapSampler = sampler_state
{
    Texture = <gDiffuseMap>;
    MINFILTER = POINT;
    MAGFILTER = POINT;
    MIPFILTER = NONE;
}

texture gEmissionMap;
sampler2D EmissionMapSampler = sampler_state
{
    Texture = <gDiffuseMap>;
    MINFILTER = POINT;
    MAGFILTER = POINT;
    MIPFILTER = NONE;
}

struct VS_IN
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float4 color    : COLOR;
    float2 texCoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 position : POSITION;  // 클립 스페이스 위치
    float3 normalW  : TEXCOORD0; // 월드 스페이스 노멀
    float4 color    : TEXCOORD1; // 정점 색상(RGBA)
    float2 texCoord : TEXCOORD2; // 텍스처 좌표
};

VS_OUT VS_Spine(VS_IN input)
{
    VS_OUT output;

    // 월드 변환
    float4 worldPos = mul(float4(input.position, 1.0f), gWorld);

    // 뷰-프로젝션
    output.position = mul(worldPos, mul(gView, gProj));

    // 월드 스페이스 노멀
    //  - (float3x3)gWorld를 통해 회전만 적용.
    //  - Spine은 2D 평면이므로 보통 (0,0,1) 고정해서 쓰지만,
    //    필요하면 실제 월드 회전 반영.
    float3 worldNormal = mul(input.normal, (float3x3)gWorld);
    output.normalW = normalize(worldNormal);

    // 정점 색 & 텍스처 좌표
    output.color    = input.color;
    output.texCoord = input.texCoord;

    return output;
}

float4 PS_Spine(VS_OUT input) : COLOR
{
    // 1) 라이트(ambient + diffuse)
    float3 N = normalize(input.normalW);
    float3 L = normalize(-gLightDirection.xyz);
    float  NdotL = saturate(dot(N, L));

    float3 lighting = gAmbientColor.rgb + (gLightColor.rgb * NdotL);

    // 2) Diffuse 텍스처 & Vertex Color
    float4 texColor = tex2D(DiffuseMapSampler, input.texCoord);

    //   - Spine 슬롯 컬러와 텍스처 알파를 곱 -> 최종 알파
    float alpha = texColor.a * input.color.a;

    //   - 알파 테스트 (clip)
    clip(alpha - 0.5f);

    //   - RGB도 Spine 정점 컬러 곱 (단, Premultiplied Alpha면 별도 처리)
    float3 finalDiffuse = texColor.rgb * input.color.rgb * gMaterialColor.rgb;

    // 3) Emission(발광) 계산 (옵션)
    float3 emission = 0;
    if (gUseEmission)
    {
        float4 emTex = tex2D(EmissionMapSampler, input.texCoord);
        // emission = emTex.rgb * gEmissionColor.rgb;
        //   - 필요하면 alpha나 premul등 추가 고려
        emission = emTex.rgb * gEmissionColor.rgb;
    }

    // 4) 최종 색상 합성: (라이팅 * Diffuse) + Emission
    float3 finalColor = lighting * finalDiffuse + emission;

    return float4(finalColor, alpha);
}

technique SpineTech
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Spine();
        PixelShader  = compile ps_3_0 PS_Spine();
    }
}
// 월드/뷰/프로젝션 행렬
float4x4 gWorld;            // 월드 행렬
float4x4 gView;
float4x4 gProj;

// 디렉셔널 라이트 파라미터
float4 gLightDirection = float4(0, -1, 0, 0); // 라이트 방향 (월드 스페이스)
float4 gLightColor     = float4(1, 1, 1, 1);  // 라이트 색상(RGB), A=1

// 앰비언트(환경광) / 오브젝트 머티리얼 색
float4 gAmbientColor   = float4(0.2, 0.2, 0.2, 1.0);
float4 gMaterialColor  = float4(1,1,1,1);

bool gEnableBillboard;

// 텍스처 (Diffuse Map)
texture gDiffuseMap;

sampler2D DiffuseMapSampler = sampler_state
{
    Texture = <gDiffuseMap>;
    MINFILTER = POINT;
    MAGFILTER = POINT;
    MIPFILTER = NONE;
};

struct VS_IN
{
    float3 position : POSITION;  // 정점 위치
    float3 normal   : NORMAL;    // 정점 노멀
    float2 texCoord : TEXCOORD0; // 텍스처 좌표
};

struct VS_OUT
{
    float4 position : POSITION;  // 클립 스페이스 위치
    float3 worldPos : TEXCOORD0; // 월드 좌표
    float3 normalW  : TEXCOORD1; // 월드 스페이스 노멀
    float2 texCoord : TEXCOORD2; // 텍스처 좌표
};

VS_OUT VS_DirLight(VS_IN input)
{
    VS_OUT output;

    // 월드 변환
    float4 wPos = mul(float4(input.position, 1.0f), gWorld);
    // 월드 좌표 → 뷰 → 프로젝션
    output.position = mul(wPos, mul(gView, gProj));

    // 월드 스페이스 정보
    output.worldPos = wPos.xyz;
    output.normalW  = normalize(mul(input.normal, (float3x3)gWorld));

    // 텍스처 좌표
    output.texCoord = input.texCoord;

    return output;
}

float4 PS_DirLight(VS_OUT input) : COLOR
{
    // 1) 기본 조명 계산(동일)
    float3 finalColor = gAmbientColor.rgb;
    float3 N = normalize(input.normalW);
    float3 L = normalize(-gLightDirection.xyz);
    float NdotL = saturate(dot(N, L));
    finalColor += gLightColor.rgb * NdotL;

    // 2) 텍스처 샘플링
    float4 texColor = tex2D(DiffuseMapSampler, input.texCoord);

    //   ※ 알파 테스트 (clip) 추가!
    //     예: 알파값 0.5 미만이면 픽셀 폐기
    clip(texColor.a - 0.5f);

    // 3) 머티리얼 컬러 곱
    float3 objectColor = texColor.rgb * gMaterialColor.rgb;

    return float4(finalColor * objectColor, 1.0f);
}

technique DirLightOnly
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_DirLight();
        PixelShader  = compile ps_3_0 PS_DirLight();
    }
}
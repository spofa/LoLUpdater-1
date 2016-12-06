//++++++++++++++++++++++++++++++++++++++++++++
// ENBSeries effect file
// visit http://enbdev.com for updates
// Copyright (c) 2007-2012 Boris Vorontsov
//++++++++++++++++++++++++++++++++++++++++++++

//global variable externally set
float4	BloomParameters; //BloomRadius1, BloomRadius2, BloomBlueShiftAmount, BloomContrast

float4	TempParameters;
float4	ScreenSize;
//x=generic timer in range 0..1, period of 16777216 ms (4.6 hours)
//w=frame time elapsed (in seconds)



//quad
struct VS_OUTPUT_POST
{
	float4 vpos  : POSITION;
	float2 txcoord0 : TEXCOORD0;
};
struct VS_INPUT_POST
{
	float3 pos  : POSITION;
	float2 txcoord0 : TEXCOORD0;
};



texture2D texBloom1;
texture2D texBloom2;
texture2D texBloom3;
texture2D texBloom4;
texture2D texBloom5;

sampler2D SamplerBloom1 = sampler_state
{
    Texture   = <texBloom1>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;//NONE;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};

sampler2D SamplerBloom2 = sampler_state
{
    Texture   = <texBloom2>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;//NONE;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};

sampler2D SamplerBloom3 = sampler_state
{
    Texture   = <texBloom3>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;//NONE;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};

sampler2D SamplerBloom4 = sampler_state
{
    Texture   = <texBloom4>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;//NONE;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};

sampler2D SamplerBloom5 = sampler_state
{
    Texture   = <texBloom5>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;//NONE;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
VS_OUTPUT_POST VS_Bloom(VS_INPUT_POST IN)
{
	VS_OUTPUT_POST OUT;

	OUT.vpos=float4(IN.pos.x,IN.pos.y,IN.pos.z,1.0);

	OUT.txcoord0.xy=IN.txcoord0.xy+TempParameters.xy;//1.0/(bloomtexsize*2.0)

	return OUT;
}


//zero pass HQ, input texture is fullscreen
//SamplerBloom1 - fullscreen texture
float4 PS_BloomPrePass(VS_OUTPUT_POST In) : COLOR
{
	float4 bloomuv;

	float4 bloom=tex2D(SamplerBloom1, In.txcoord0);
	const float2 offset[8]=
	{
/*		float2(0.707, 0.707),
		float2(0.707, -0.707),
		float2(-0.707, 0.707),
		float2(-0.707, -0.707),*/
		float2(1.0, 1.0),
		float2(0.0, -1.0),
		float2(-1.0, 1.0),
		float2(-1.0, -1.0),
		float2(0.0, 1.0),
		float2(0.0, -1.0),
		float2(1.0, 0.0),
		float2(-1.0, 0.0)
	};
	float2 screenfact=1.0;
	screenfact.y*=ScreenSize.z;
	screenfact.xy*=TempParameters.z*1.5;
	float4 srcbloom=bloom;
	for (int i=0; i<8; i++)
	{
		bloomuv.xy=offset[i];
		bloomuv.xy=(bloomuv.xy*screenfact.xy)+In.txcoord0.xy;//-(1.0/256.0);//-(1.0/512.0);
		bloom+=tex2D(SamplerBloom1, bloomuv.xy);
	}
	bloom*=0.111111;//0.125;

	bloom.w=1.0;
	return bloom;
}



//first and second passes draw to every texture
//twice, after computations of these two passes,
//result is set as input to next cycle

//first pass
//SamplerBloom1 is result of prepass or second pass from cycle
float4 PS_BloomTexture1(VS_OUTPUT_POST In) : COLOR
{
	float4 bloomuv;

	float4 bloom=tex2D(SamplerBloom1, In.txcoord0);
	const float2 offset[8]=
	{
		float2(0.707, 0.707),
		float2(0.707, -0.707),
		float2(-0.707, 0.707),
		float2(-0.707, -0.707),
		float2(0.0, 1.0),
		float2(0.0, -1.0),
		float2(1.0, 0.0),
		float2(-1.0, 0.0)
	};
	float2 screenfact=1.0;
	screenfact.y*=ScreenSize.z;
	screenfact.xy/=ScreenSize.x;
	float4 srcbloom=bloom;
	float step=(TempParameters.w-0.5);//*1.5;
	screenfact.xy*=step;

	float4 bloomadd=bloom;
	for (int i=0; i<8; i++)
	{
		bloomuv.xy=offset[i]*BloomParameters.x;
		bloomuv.xy=(bloomuv.xy*screenfact.xy)+In.txcoord0.xy;//-(1.0/256.0);//-(1.0/512.0);
		//v1
//		bloomadd+=tex2D(SamplerBloom1, bloomuv.xy);
		//v2
		float4 tempbloom=tex2D(SamplerBloom1, bloomuv.xy);
		bloomadd+=tempbloom;
//		float fgr=dot(bloom.xyz, 0.333);
		bloom.xyz=max(bloom.xyz, tempbloom.xyz*0.99);
	}
	//v1
	bloomadd*=0.111111;
	//v0
	bloom.xyz=lerp(bloomadd.xyz, bloom.xyz, BloomParameters.w);


	//float3 violet=float3(0.78, 0.5, 1.0);
	//float3 violet=float3(0.6, 0.4, 1.0);//v2
	float3 violet=float3(0.6, 0.5, 1.0);//v3

	//this applies when white
	//float gray=0.104*dot(srcbloom.xyz, 0.333);//max(srcbloom.x, max(srcbloom.y, srcbloom.z));
	//this applies on dark and when contrast
	float ttt=dot(bloom.xyz, 0.333)-dot(srcbloom.xyz, 0.333);
	ttt=max(ttt, 0.0);
	float gray=BloomParameters.z*ttt;//max(srcbloom.x, max(srcbloom.y, srcbloom.z));
	float mixfact=(gray/(1.0+gray));
	mixfact*=1.0-saturate((TempParameters.w-1.0)*0.3);
	violet.xy+=saturate((TempParameters.w-1.0)*0.3);
	violet.xy=saturate(violet.xy);
	bloom.xyz*=lerp(1.0, violet.xyz, mixfact);

	bloom.w=1.0;
	return bloom;
}


//second pass
//SamplerBloom1 is result of first pass
float4 PS_BloomTexture2(VS_OUTPUT_POST In) : COLOR
{
	float4 bloomuv;

	float4 bloom=tex2D(SamplerBloom1, In.txcoord0);
	const float2 offset[8]=
	{
		float2(0.707, 0.707),
		float2(0.707, -0.707),
		float2(-0.707, 0.707),
		float2(-0.707, -0.707),
		float2(0.0, 1.0),
		float2(0.0, -1.0),
		float2(1.0, 0.0),
		float2(-1.0, 0.0)
	};
	float2 screenfact=1.0;
	screenfact.y*=ScreenSize.z;
	screenfact.xy/=ScreenSize.x;
	float4 srcbloom=bloom;

	float step=(TempParameters.w-0.5)*1.2;//v3
	screenfact.xy*=step;
	float4 rotvec=0.0;
	sincos(0.19635, rotvec.x, rotvec.y);
	for (int i=0; i<8; i++)
	{
		bloomuv.xy=offset[i];
		bloomuv.xy=reflect(bloomuv.xy, rotvec.xy);
		bloomuv.xy*=BloomParameters.y;
		//separate code is much faster without constant table operations
		bloomuv.xy=(bloomuv.xy*screenfact.xy)+In.txcoord0.xy;//-(1.0/256.0);//-(1.0/512.0);
		bloom+=tex2D(SamplerBloom1, bloomuv.xy);
	}
	bloom*=0.111111;//0.125;

	bloom.w=1.0;
	return bloom;
}



//last pass, mix several bloom textures
//SamplerBloom5 is the result of prepass
float4 PS_BloomPostPass(VS_OUTPUT_POST In) : COLOR
{
	float4 bloom;

	//v1
	bloom =tex2D(SamplerBloom1, In.txcoord0);
	bloom+=tex2D(SamplerBloom2, In.txcoord0);
	bloom+=tex2D(SamplerBloom3, In.txcoord0);
	bloom+=tex2D(SamplerBloom4, In.txcoord0);
	bloom+=tex2D(SamplerBloom5, In.txcoord0);
	bloom*=0.2;
/*
	//v2
	float4 bloom1=tex2D(SamplerBloom1, In.txcoord0);
	float4 bloom2=tex2D(SamplerBloom2, In.txcoord0);
	float4 bloom3=tex2D(SamplerBloom3, In.txcoord0);
	float4 bloom4=tex2D(SamplerBloom4, In.txcoord0);
	float4 bloom5=tex2D(SamplerBloom5, In.txcoord0);
	bloom=max(bloom1, bloom2);
	bloom=max(bloom, bloom3);
	bloom=max(bloom, bloom4);
	bloom=max(bloom, bloom5);
*/

	bloom.w=1.0;
	return bloom;
}



technique BloomPrePass
{
    pass p0
    {
	VertexShader = compile vs_3_0 VS_Bloom();
	PixelShader  = compile ps_3_0 PS_BloomPrePass();

	COLORWRITEENABLE=ALPHA|RED|GREEN|BLUE;
	CullMode=NONE;
	AlphaBlendEnable=FALSE;
	AlphaTestEnable=FALSE;
	SEPARATEALPHABLENDENABLE=FALSE;
	FogEnable=FALSE;
	SRGBWRITEENABLE=FALSE;
	}
}

technique BloomTexture1
{
    pass p0
    {
	VertexShader = compile vs_3_0 VS_Bloom();
	PixelShader  = compile ps_3_0 PS_BloomTexture1();

	COLORWRITEENABLE=ALPHA|RED|GREEN|BLUE;
	CullMode=NONE;
	AlphaBlendEnable=FALSE;
	AlphaTestEnable=FALSE;
	SEPARATEALPHABLENDENABLE=FALSE;
	FogEnable=FALSE;
	SRGBWRITEENABLE=FALSE;
	}
}


technique BloomTexture2
{
    pass p0
    {
	VertexShader = compile vs_3_0 VS_Bloom();
	PixelShader  = compile ps_3_0 PS_BloomTexture2();

	COLORWRITEENABLE=ALPHA|RED|GREEN|BLUE;
	CullMode=NONE;
	AlphaBlendEnable=FALSE;
	AlphaTestEnable=FALSE;
	SEPARATEALPHABLENDENABLE=FALSE;
	FogEnable=FALSE;
	SRGBWRITEENABLE=FALSE;
	}
}

technique BloomPostPass
{
    pass p0
    {
	VertexShader = compile vs_3_0 VS_Bloom();
	PixelShader  = compile ps_3_0 PS_BloomPostPass();

	COLORWRITEENABLE=ALPHA|RED|GREEN|BLUE;
	CullMode=NONE;
	AlphaBlendEnable=FALSE;
	AlphaTestEnable=FALSE;
	SEPARATEALPHABLENDENABLE=FALSE;
	FogEnable=FALSE;
	SRGBWRITEENABLE=FALSE;
	}
}




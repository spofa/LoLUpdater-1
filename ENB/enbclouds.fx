//++++++++++++++++++++++++++++++++++++++++++++
// ENBSeries effect file
// visit http://enbdev.com for updates
// Copyright 2007-2010 (c) Boris Vorontsov
//
// Using decompiled shader of GTA4 game by
// Rockstar Games.
//++++++++++++++++++++++++++++++++++++++++++++

//global variable externally set
float4	CameraPosition;
//x=generic timer in range 0..1, period of 16777216 ms (4.6 hours)
//w=frame time elapsed (in seconds)
float4	Timer;



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



texture2D texColor;

sampler2D SamplerColor = sampler_state
{
	Texture   = <texColor>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;//LINEAR; //may be mipmapped?
	AddressU  = Wrap;//Clamp;
	AddressV  = Wrap;//Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//first, clouds layer texture generator
technique Shader_Clouds
{
    pass p0 
    {

	PixelShader= 
	asm
	{

// Registers:
//
//   Name          Reg   Size
//   ------------- ----- ----
//   Velocity1     c64      1
//   Velocity2     c65      1
//   Frequency1    c66      1
//   Frequency2    c67      1
//   Contrast      c68      1
//   Brightness    c69      1
//   NoiseWarp     c70      1
//   Persistance   c71      1
//   Phase         c72      1
//   ClumpBump     c73      1
//   Noise1Sampler s0       1
//

    ps_3_0
    def c0, -0.5, 0, 12, 0.5
    def c1, 0.125, 0, -1, 1

//x=generic timer in range 0..1, period of 16777216 ms (4.6 hours)
//w=frame time elapsed (in seconds)
//def c150, 0.0, 0.0, 0.0, 0.0 //external

//x=controls mix between procedural and static
//def c152, 0.0, 0.0, 0.0, 0.0 //external

//camera position
//def c153, 0.0, 0.0, 0.0, 0.0
//sun direction vector
//def c154, 0.0, 0.0, 0.0, 0.0

//x - camera movement factor
//def c170, 0.00005, 0.0, 0.0, 0.0
def c170, 0.0, 0.0, 0.0, 0.0 //disabled

    defi i0, 2, 0, 0, 0
    dcl_texcoord v0.xy
    dcl_2d s0
//external texture
dcl_2d s1

//camera movement
mov r11, c153
mov r11.y, -r11.y
mul r11, r11.xyxy, c170.x


    add r0, c0.x, r11.xyxy
    add r0, c0.x, v0.xyxy //orig
    dp2add r1.x, r0.zwzw, r0.zwzw, c0.y
    pow r2.x, r1.x, c70.x //orig
    rsq r1.x, r1.x //orig
    mul r0, r0, r1.x
    mul r0, r2.x, r0
    mad r0, r0, c0.z, c0.w
    mul r1, r0, c1.x
    mov r0.x, c72.x
    mul r2.xy, r0.x, c64
    mul r2.zw, r0.x, c65.xyxy
    mad r0, r0.zwzw, c1.x, r2
    mov r3.x, c66.x
    mov r3.z, c67.x
    mul r4, r1, r3.xxzz
    mad r1, r1, r3.xxzz, r2
add r10, r0, r11.xyxy
texld r5, r10, s0
texld r0, r10.zwzw, s0
add r10, r1, r11.xyxy
texld r6, r10, s0
texld r1, r10.zwzw, s0
/*
    texld r5, r0, s0 //orig
    texld r0, r0.zwzw, s0 //orig
    texld r6, r1, s0 //orig
    texld r1, r1.zwzw, s0 //orig
*/
    mov r5.xz, r5.xyyw
    mov r5.yw, r0.xxzy
    mov r0.w, c0.w
    mul r0.x, r0.w, c71.x
    mov r6.xz, r6.xyyw
    mov r6.yw, r1.xxzy
    dp2add r0.y, r6, r0.x, c0.y
    dp2add r0.y, r5, c0.w, r0.y
    dp2add r0.z, r6.zwzw, r0.x, c0.y
    dp2add r0.z, r5.zwzw, c0.w, r0.z
    mul r1, r3.xxzz, r4
    mul r0.x, r0.x, c71.x
    mov r4, r1
    mov r5.xy, r0.yzzw
    mov r0.w, r0.x
    rep i0
      add r6, r2, r4
      mul r7, r3.xxzz, r4
      mad r8, r4, r3.xxzz, r2

add r10, r6, r11.xyxy
texld r9, r10, s0
texld r6, r10.zwzw, s0
add r11, r8, r11.xyxy
texld r10, r11, s0
texld r8, r11.zwzw, s0
/*
      texld r9, r6, s0 //orig
      texld r6, r6.zwzw, s0 //orig
      texld r10, r8, s0 //orig
      texld r8, r8.zwzw, s0 //orig
*/
      mov r9.xz, r9.xyyw
      mov r9.yw, r6.xxzy
      dp2add r3.y, r9, r0.w, r5.x
      dp2add r3.w, r9.zwzw, r0.w, r5.y
      mul r5.w, r0.w, c71.x
      mov r6.xz, r10.xyyw
      mov r6.yw, r8.xxzy
      dp2add r5.x, r6, r5.w, r3.y
      dp2add r5.y, r6.zwzw, r5.w, r3.w
      mul r4, r3.xxzz, r7
      mul r0.w, r5.w, c71.x
    endrep
    add r0.x, -r0.y, r5.x
    add r5.z, r0.x, r0.x
    add_sat r0.xyz, r5, -c68.x
    mul r0.xy, r0, c69.x
    mov r1.zw, c1
    mad r0.z, r0.z, c69.x, r1.z
    mad r0.w, c73.x, r0.z, r1.w
//    mov_sat oC0.xyz, r0.xyww //orig
    mov oC0.w, c1.w


//bypass texture if enbclouds.bmp is found

//camera movement
//mov r11, c153
mov r11.y, -r11.y
mul r11, r11.xyxy, c170.x
add r11.xy, r11, v0.xy

    texld r5, r11, s1
    lrp r10.xyz, c152.x, r5, r0.xyww
    mov_sat oC0.xyz, r10


	};
    }
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//second, make some shift at horizon for clouds texture
//render target texture is 512*512 pixels
VS_OUTPUT_POST VS_Quad(VS_INPUT_POST IN)
{
	VS_OUTPUT_POST OUT;

	OUT.vpos=float4(IN.pos.x,IN.pos.y,IN.pos.z,1.0);
	OUT.txcoord0.xy=IN.txcoord0.xy;

	return OUT;
}

float4 PS_ShiftClouds(VS_OUTPUT_POST IN) : COLOR
{
/*	float2 camshift=CameraPosition.xy;
	camshift.y=-camshift.y;
	camshift*=0.00005;
*/
	float4 res;
	float4 uvsrc;
	uvsrc.xy=IN.txcoord0.xy;
	uvsrc.z=0.0;
	uvsrc.w=0.0;
/*
	float4 dir=0.0;
	dir.xy=(uvsrc.xy-0.5);
	//dir.xy=abs(pow(dir.xy, 2.0))*dir.xy;

	res=tex2Dbias(SamplerColor, uvsrc);// + (camshift*0.005);
	float4 uvtemp=0.0;
	for (int i=0; i<4; i++)
	{
		uvtemp.xy=IN.txcoord0.xy;// + camshift;
		uvtemp.xy+=dir.xy*0.1*tempF1.y;//offset[i];
		float tres=tex2Dbias(SamplerColor, uvtemp);
//		res=max(res, tres);
		res=lerp(res, tres, saturate(tres));
//		res+=tres;
	}
//	res*=0.2;
*/

//bypass
	uvsrc.xy=IN.txcoord0.xy;// + camshift;
	uvsrc.w=0.0;
	res=tex2D(SamplerColor, uvsrc.xy);

	res.w=1.0;
	return res;
}



technique ShiftClouds
{
    pass p0
    {
	VertexShader = compile vs_3_0 VS_Quad();
	PixelShader  = compile ps_3_0 PS_ShiftClouds();

	COLORWRITEENABLE=ALPHA|RED|GREEN|BLUE;
	CullMode=NONE;
	AlphaBlendEnable=FALSE;
	AlphaTestEnable=FALSE;
	SEPARATEALPHABLENDENABLE=FALSE;
	FogEnable=FALSE;
	SRGBWRITEENABLE=FALSE;
    }
}



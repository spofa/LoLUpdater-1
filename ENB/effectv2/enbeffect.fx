//++++++++++++++++++++++++++++++++++++++++++++
// ENBSeries effect file
// visit http://enbdev.com for updates
// Copyright 2010 (c) Boris Vorontsov
//
// Using decompiled shader of GTA4 game by
// Rockstar Games.
//++++++++++++++++++++++++++++++++++++++++++++

//NOTE: enable or disable these
//to disable, write // symbols before #define
#define POSTPROCESS_V2 //overwise v1 of color processing
//#define APPLYGAMECOLORCORRECTION //use contrast, brightness, saturation from game

float	EColorSaturation=1.0; //overall for two methods

float	EBrightnessV1=2.0;
float	EBloomAmountV1=0.25;
float	EBloomAmountAdditiveV1=0.8;

float	EBrightnessV2=0.06;
float	EBloomAmountV2=0.04;
float	EColoringV2=0.5;
float	EDarkeningV2=0.4;



//   sampler2D AdapLumSampler;
//   sampler2D BloomSampler;
//   float4 ColorCorrect;
//   float4 ColorShift;
//   float Exposure;
//   sampler2D GBufferTextureSampler2;
//   sampler2D GBufferTextureSampler3;
//   sampler2D HDRSampler;
//   sampler2D JitterSampler;
//   float PLAYER_MASK;
//   sampler2D StencilCopySampler;
//   float4 TexelSize;
//   float4 ToneMapParams;
//   float4 deSatContrastGamma;
//   float4 dofBlur;
//   float4 dofDist;
//   float4 dofProj;
//   float gDirectionalMotionBlurLength;
//   float4 globalScreenSize;
//   row_major float4x4 motionBlurMatrix;
//
//
// Registers:
//
//   Name                         Reg   Size
//   ---------------------------- ----- ----
//   globalScreenSize             c44      1
//   Exposure                     c66      1
//   motionBlurMatrix             c72      4
//   TexelSize                    c76      1
//   dofProj                      c77      1
//   dofDist                      c78      1
//   dofBlur                      c79      1
//   gDirectionalMotionBlurLength c80      1
//   ToneMapParams                c81      1
//   deSatContrastGamma           c82      1
//   ColorCorrect                 c83      1
//   ColorShift                   c84      1
//   PLAYER_MASK                  c85      1
//   GBufferTextureSampler2       s0       1
//   GBufferTextureSampler3       s1       1
//   HDRSampler                   s2       1
//   BloomSampler                 s3       1
//   AdapLumSampler               s4       1
//   JitterSampler                s5       1
//   StencilCopySampler           s6       1

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

//int4	_i0 : register(i0);

/*
float4	_c0 : register(c0);
float4	_c1 : register(c1);
float4	_c2 : register(c2);
float4	_c3 : register(c3);
float4	_c4 : register(c4);
float4	_c5 : register(c5);
float4	_c6 : register(c6);
*/
float4	_c44 : register(c44);
float4	_c66 : register(c66);
float4	_c72 : register(c72);
float4	_c73 : register(c73);
float4	_c74 : register(c74);
float4	_c75 : register(c75);
float4	_c76 : register(c76);
float4	_c77 : register(c77);
float4	_c78 : register(c78);
float4	_c79 : register(c79);
float4	_c80 : register(c80);
float4	_c81 : register(c81);
float4	_c82 : register(c82);
float4	_c83 : register(c83);
float4	_c84 : register(c84);
float4	_c85 : register(c85);


texture2D texs0 : register(s0);
texture2D texs1 : register(s1);
texture2D texs2 : register(s2);
texture2D texs3 : register(s3);
texture2D texs4 : register(s4);
texture2D texs5 : register(s5);
texture2D texs6 : register(s6);
texture2D texs7 : register(s7);
texture2D texs15 : register(s15);
//sampler2D s0=sampler_state { Texture=<texs0>; };
//sampler2D s1=sampler_state { Texture=<texs1>; };
sampler2D s2=sampler_state { Texture=<texs2>; };
//sampler2D s3=sampler_state { Texture=<texs3>; };
sampler2D s4=sampler_state { Texture=<texs4>; };
sampler2D s5=sampler_state { Texture=<texs5>; };
sampler2D s6=sampler_state { Texture=<texs6>; };
sampler2D s7=sampler_state { Texture=<texs7>; };
sampler2D s15=sampler_state { Texture=<texs15>; };

//next samplers states fixing problems on ATI cards
sampler2D s0 = sampler_state
{
    Texture   = <texs0>;
	MinFilter = LINEAR;//
	MagFilter = LINEAR;//
	MipFilter = NONE;//NONE;//LINEAR;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};

sampler2D s1 = sampler_state
{
    Texture   = <texs1>;
	MinFilter = LINEAR;//
	MagFilter = LINEAR;//
	MipFilter = NONE;//NONE;//LINEAR;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};

sampler2D s3 = sampler_state
{
    Texture   = <texs3>;
	MinFilter = LINEAR;//
	MagFilter = LINEAR;//
	MipFilter = NONE;//NONE;//LINEAR;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
float4 PS_C215BE6E(VS_OUTPUT_POST IN) : COLOR
{
	float4 _oC0=0.0; //output

	float4 _c0=float4(0, 0.212500006, 0.715399981, 0.0720999986);
	float4 _c1=float4(0.25, 1, 256, 0);
	float4 _c2=float4(0, 2, 4, 8);
	float4 _c3=float4(58.1640015, 47.1300011, 0, 0);
	float4 _c4=float4(-0.5, -1.5, 1.5, 0.5);
	float4 _c5=float4(2, -1, 0.125, 0);
	float4 _c6=float4(256, 2, 4, 8);
//mine
	float4 _c7=float4(1.0, 0.1, 0.8, 1.0);
	float4 _c8=float4(0.0, 0.1, 0.8, 0.333);
	float4 _c9=float4(0.06, 0.1, 0.8, 0.1);
	float4 _c10=float4(0.01, 1.4, 0.0, 0.32);
	float4 _c11=float4(0.333333, 1.0, 0.0, 0.0);
	float4 _c12=float4(0.27, 0.67, 0.06, 0.0);
	float4 _c13=float4(0.0, 0.0, 0.0, 0.00001);
	float4 _c150=float4(0.55, 0.9, 0.85, 0.9);
	float4 _c195=float4(0.0, 0.0, 0.0, 0.07);//0.7


	float4 r0;
	float4 r1;
	float4 r2;
	float4 r3;
	float4 r4;
	float4 r5;
	float4 r6;
	float4 r7;
	float4 r8;
	float4 r9;
	float4 r10;
	float4 r11;


	float4 _v0=0.0;
	_v0.xy=IN.txcoord0.xy;





	r0=tex2D(s1, _v0.xy); //GBufferTextureSampler3
	r0.y=-_c77.x + _c77.y; //-dofProj.x + dofProj.y
	r0.y=1.0/r0.y;
	r0.z=r0.y * _c77.y; //dofProj.y
	r0.z=r0.z * -_c77.x; //dofProj.x
	r0.x=_c77.y * -r0.y + r0.x;
	r0.x=1.0/r0.x;
	r0.y=r0.z * r0.x;

	r1=tex2D(s2, _v0.xy); //HDRSampler
	r2=tex2D(s3, _v0.xy); //BloomSampler

//r1*=10.0;

	r0.w=abs(_c79.w);
	r3=_c4; //-0.5, -1.5, 1.5, 0.5

//TEMP decrease bluriness for edge smoothing
float2 off_c76=_c76.xy*0.2;

	r4.xy=off_c76.xy * r3.xy + _v0.xy; //TexelSize.xy
	r4=tex2D(s2, r4.xy); //HDRSampler

	r5.xy=off_c76.xy * r3.zx + _v0.xy; //TexelSize.xy
	r5=tex2D(s2, r5.xy); //HDRSampler

	r3.xz=off_c76.xy * r3.wz + _v0.xy; //TexelSize.xy
	r6=tex2D(s2, r3.xz); //HDRSampler

	r3.xy=off_c76.xy * r3.yw + _v0; //TexelSize.xy
	r7=tex2D(s2, r3.xy); //HDRSampler


	r1.w=dot(r4.xyz, r4.xyz);
	r1.w=1.0/r1.w;
	if (-r1.w<0.0)
	{
		r3.xyz=r4.xyz;
	} else r3.xyz=_c0.x;

	r1.w=dot(r5.xyz, r5.xyz); //dp3
	r1.w=1.0/r1.w;
	if (-r1.w<0.0)
	{
		r4.xyz=r5.xyz;
	} else r4.xyz=_c0.x;

	r1.w=dot(r6.xyz, r6.xyz); //dp3
	r1.w=1.0/r1.w;
	if (-r1.w<0.0)
	{
		r5.xyz=r6.xyz;
	} else r5.xyz=_c0.x;

	r1.w=dot(r7.xyz, r7.xyz); //dp3
	r1.w=1.0/r1.w;
	if (-r1.w<0.0)
	{
		r6.xyz=r7.xyz;
	} else r6.xyz=_c0.x;

	r1.w=dot(r1.xyz, r1.xyz); //dp3
	r1.w=1.0/r1.w;
	if (-r1.w<0.0)
	{
		r7.xyz=r1.xyz;
	} else r7.xyz=_c0.x;

	r0.x=r0.z * r0.x - _c78.w; //dofDist.w
	r0.x=_c78.y * -r3.w + r0.x; //dofDist.y
	r1.w=max(r0.x, _c0.x); //0.0
	r0.x=1.0/_c78.z; //dofDist.z
	r0.x=r1.w * r0.x;
	r1.w=lerp(_c79.y, _c79.z, r0.x);//dofBlur
	r0.x=min(_c79.z, r1.w); //dofBlur.z

	r0.z=dot(r7.xyz, _c0.yzw); //0.212500006, 0.715399981, 0.0720999986
	r8.x=dot(r3.xyz, _c0.yzw); //0.212500006, 0.715399981, 0.0720999986
	r8.y=dot(r4.xyz, _c0.yzw); //0.212500006, 0.715399981, 0.0720999986
	r8.z=dot(r5.xyz, _c0.yzw); //0.212500006, 0.715399981, 0.0720999986
	r8.w=dot(r6.xyz, _c0.yzw); //0.212500006, 0.715399981, 0.0720999986
	r1.w=dot(r8, _c1.xxxx); //0.25

	r8=r8 - r1.w;
	r2.w=dot(r8, r8);
	r0.z=r0.z - r1.w;
	r0.z=r0.z * r0.z - r2.w;
	r0.x=r0.x*r0.x;
	if (r0.z<0.0)
	{
		r0.x=r0.x;
	} else r0.x=_c1.y; //1.0


	r8=tex2D(s0, _v0.xy); //GBufferTextureSampler2


	r8.yz=_c1.yz; // 1.0, 256.0
	r9=r8.w * -r8.z + _c2; //0, 2, 4, 8
	if (r9.w<0.0) //r9<0.0
	{
		r9=_c1.w; //0.0
	} else r9=_c1.y; //1.0
	//r9 is filtered sky mask


	//r10=r8.w + tempc6;//c6.yzwx; //256, 2, 4, 8
	r10=r8.w * -_c6.x + _c6.yzwx; //256, 2, 4, 8


	if (r10.x<0.0)
	{
		r10.x=-_c1.w; //0.0
	} else r10.x=-_c1.y; //1.0

	if (r10.y<0.0)
	{
		r10.y=-_c1.w; //0.0
	} else r10.y=-_c1.y; //1.0

	if (r10.z<0.0)
	{
		r10.z=-_c1.w; //0.0
	} else r10.z=-_c1.y; //1.0

	if (r10.w<0.0)
	{
		r10.w=-_c1.w; //0.0
	} else r10.w=-_c1.y; //1.0


	r9=r9 + r10;
	r9=r9 * _c4.x; //-0.5


	r0.z=dot(r9, _c1.yyyy); //1.0
	r0.z=r0.z + _c4.w; //0.5
	r0.z=1.0/r0.z;
	r8.xzw=r3.xyz * r9.x;


	r7.xyz=r7 * _c4.w + r8.xzw; //0.5
	r7.xyz=r4 * r9.y + r7;
	r7.xyz=r5 * r9.z + r7;
	r7.xyz=r6 * r9.w + r7;
	r7.xyz=r0.z * r7;
	r0.z=-r0.x + _c1.y; //1.0
	r1.w=r0.x * _c1.x; //0.25


	r3.xyz=r3 * r1.w;
	r3.xyz=r7 * r0.z + r3;
	r3.xyz=r4 * r1.w + r3;
	r3.xyz=r5 * r1.w + r3;
	r3.xyz=r6 * r1.w + r3;


	//r4.xyz=_v0.yxy * _c5.x + _c5.y; //2, -1
	r4.x=_v0.y * _c5.x + _c5.y;
	r4.y=_v0.x * _c5.x + _c5.y;
	r4.z=_v0.y * _c5.x + _c5.y;

	r0.z=r4.y * _c77.z; //dofProj.z
	r0.z=r0.y * r0.z;
	r1.w=-r4.x * _c77.w; //dofProj.w
	r1.w=r0.y * r1.w;
	r5.xyz=r1.w * _c73; //motionBlurMatrix.2
	r5.xyz=r0.z * _c72 + r5; //motionBlurMatrix.1
	r5.xyz=-r0.y * _c74 + r5; //motionBlurMatrix.3
	r5.xyz=r5 + _c75; //motionBlurMatrix.4
	r0.y=-r5.z * _c77.z; //dofProj.z
	r0.y=1.0/r0.y;
	r6.x=r5.x * r0.y;
	r0.y=r5.z * _c77.w; //dofProj.w
	r0.y=1.0/r0.y;
	r6.y=r5.y * r0.y;
	//r0.yz=-r4 + r6.xxyw;
	r0.yz=-r4.yz + r6.xy;

	//r0.yz=r0 * _c80.x; //gDirectionalMotionBlurLength.x
	r0.y=r0.y * _c80.x;
	r0.z=r0.z * _c80.x;
	//r4.xy=r0.yzzw * _c5.z; //0.125
	r4.x=r0.y * _c5.z;
	r4.y=r0.z * _c5.z;
	r5=tex2D(s6, _v0.xy); //StencilCopySampler
	r1.w=r5.x + -_c85.x; //PLAYER_MASK.x
	r4.zw=r3.xyxy * _c2.w; //8.0
	//r4.z=r3.x * _c2.w;
	//r4.w=r3.y * _c2.w;
	//r4.zw=_v0.xyxy * _c3.xyxy + r4; //58.1640015, 47.1300011
	r4.z=_v0.x * _c3.x + r4.z; //v2
	r4.w=_v0.y * _c3.y + r4.w; //v2
	r5=tex2D(s5, r4.zw); //JitterSampler
	r2.w=r5.x + _c4.x; //-0.5
	//r4.zw=r4.xyxy * r2.w + _v0.xyxy; //v0
	r4.z=r4.x * r2.w + _v0.x; //v2
	r4.w=r4.y * r2.w + _v0.y; //v2
	r5.xyz=r3.xyz;
	r2.w=_c1.y; //1.0
	r3.w=_c1.y; //1.0

//motion blur
	for (int iii=0; iii<7; iii++)
	{
	//	r6.xy=r4 * r3.w + r4.zwzw; //v0
	r6.x=r4.x * r3.w + r4.z; //v2
	r6.y=r4.y * r3.w + r4.w; //v2
		r7=tex2D(s6, r6.xy);
		r5.w=r7.x - _c85.x;
		if (r5.w<0.0)
		{
			r5.w=_c1.y; //1.0
		} else r5.w=_c1.w; //0.0
		r6=tex2D(s2, r6.xy);
		r5.xyz=r6.xyz * r5.w + r5.xyz;
		r2.w=r2.w + r5.w;
		r3.w=r3.w + _c1.y; //1.0
	}



	r2.w=1.0/r2.w;

	//r0.yz=r0 * _c44.xxyw; //globalScreenSize
	r0.y=r0.y * _c44.x; //globalScreenSize
	r0.z=r0.z * _c44.y; //globalScreenSize

	r0.y=dot(r0.yz, r0.yz)+_c0.x;//dp2

	//r0.y=sqrt(r0.y);
	//r0.y=1.0/r0.y;
	r0.y=pow(r0.y, _c4.w); //0.5

	r0.y=r0.y * _c4.w; //0.5
	r0.x=r0.x + _c1.y; //1.0
	r0.x=1.0/r0.x;
	//r0.y is wrong
	r0.x=saturate(r0.y * r0.x);
	r4.xyz=r5 * r2.w - r3;
	r0.xyz=r0.x * r4 + r3;

	//_oC0.xyz=0.5*r0.xyz;

	if (-r1.w<0.0) r0.xyz=r3.xyz;
	if (-r0.w<0.0) r0.xyz=r1.xyz;

//for future use
r11=r0;


	r1=tex2D(s4, _c0.x); //AdapLumSampler , 0.0
	r0.w=1.0/r1.x;
	r0.w=r0.w * _c81.y; //ToneMapParams.y
	r1.x=1.0/r0.w;
	r1.x=r1.x * _c81.x; //ToneMapParams.x
	r1.xyz=r2 * _c66.x - r1.x; //Exposure.x
	r2.xyz=max(r1, _c0.x); //0.0
	r1.xyz=r2 * _c81.z; //ToneMapParams.z
	r1.xyz=r1 * _c1.x; //0.25
	r0.xyz=r0 * _c66.x + r1; //Exposure.x
	r1.xyz=r0.w * r0;
	r1.x=dot(r1.xyz, _c0.yzw); //0.2125, 0.7154, 0.0721
	r0.xyz=r0 * r0.w - r1.x;
	r0.xyz=_c82.x * r0 + r1.x; //deSatContrastGamma.x
	r0.w=r1.x * _c84.w; //ColorShift.w
	r1.yzw=r0.w * _c84.xxyz; //ColorShift

	//darken as DarkeningAmount, but keeps colors
	r2.x=saturate(r0.w);
	r0.xyz=r0.xyz - _c84.xyz * r0.w; //ColorShift
	r0.xyz=r2.x * r0.xyz + r1.yzww;

	//coloring (green, brown, ...)
	r0.xyz=2.0 * r0.xyz * _c83.xyz; //ColorCorrect

	//keeps colors, changes luma
	r1.x=saturate(r1.x);//luma
	r0.w=_c82.z - _c1.y; //deSatContrastGamma.z - 1.0
	r2.x=pow(r1.x, r0.w);

	_oC0.xyz=r0.xyz * r2.x; //orig
	_oC0.w=_c1.y; //1.0





#ifndef POSTPROCESS_V2

	r5=tex2D(s4, _c0.x);//adaptation luminance
	//r5=1.0;

	float4 color;
	//r0=tex2D(s2, _v0.xy);//hdr color
	color=r11; //FROM ORIGINAL CODE AFTER BLUR
	float	luma=0.27*color.r + 0.67*color.g + 0.06*color.b;

//increase color
	float cgray=dot(color.xyz, 0.333);//luma
	float3 poweredcolor=pow(color.xyz, EColorSaturation);
	float newgray=dot(poweredcolor.xyz, 0.333);
	color.xyz=poweredcolor.xyz*cgray/(newgray+0.0001);

	//adaptation
	r5.w=r5.x;
	r5.w=1.0/r5.w;
	color.xyz=color.xyz * r5.w;
	r3.xy=_c10.xwxw;
	r3.xy=r3.xy * r5.w;

	float4 colorbloom=tex2D(s3, _v0.xy); //bloom

	//HERE 0.15 is bloom factor 1, soft bloom
	color=colorbloom * EBloomAmountV1 + color;
	//power from intensity

	r10=dot(color.xyz, _c12.xyz); //luminance

	r9.x=r10.x * EBrightnessV1;//c199.x;
	r10.x=1.0/r10.x;
	r10=r10 * r9.x;
	color=color * r10.x;
	color=color * _c7.y; //pre multiply
	r1=color * _c7.z; //e multiply
	r1=r1 + _c7.w;

	color=color / (r1 + _c7.w); //+1.0

	//HERE 0.8 is bloom for additive mix
	color=EBloomAmountAdditiveV1*colorbloom * r3.x + color;

#endif //not POSTPROCESS_V2


#ifdef POSTPROCESS_V2

	float4 color=r11;
	float	luma=0.27*color.r + 0.67*color.g + 0.06*color.b;

//increase color
	float cgray=dot(color.xyz, 0.333);//luma
	float3 poweredcolor=pow(color.xyz, EColorSaturation);
	float newgray=dot(poweredcolor.xyz, 0.333);
	color.xyz=poweredcolor.xyz*cgray/(newgray+0.0001);



	r5=tex2D(s4, _c0.x);//adaptation luminance
	r5.w=r5.x;
	r5.w=1.0/r5.w;
	color.xyz=color.xyz * r5.w;


	color.rgb*=EBrightnessV2;//0.07
	float4 colorbloom=r5.w*EBloomAmountV2*tex2D(s3, _v0.xy);//0.5
	float bloomgray=dot(colorbloom.xyz, 0.333);
	//color.xyz=lerp(color.xyz, color.xyz*0.55+colorbloom.xyz*0.2, saturate(bloomgray));
	color.xyz=lerp(color.xyz, color.xyz*0.8+colorbloom.xyz*0.2, saturate(bloomgray));
	//color.xyz=(bloomgray)*color.xyz + saturate(1.0-(bloomgray))*bloom.xyz;

	//color.xyz*=color.xyz;
	color.xyz=max(color.xyz, 0.0001);
	//color.rgb*=tempF1.w;
	//color.rgb=pow(color.xyz, pregamma);

	float	colorgray=dot(color.xyz, 0.333);//luma//max(color.x, max(color.y, color.z));//
	float3	middlecolor=(color.xyz+0.0001)/(colorgray+0.000001);
	middlecolor.xyz=lerp(middlecolor.xyz, dot(middlecolor.xyz, 0.333), EColoringV2);//0.6 //color
	color.xyz=middlecolor.xyz*pow(colorgray, EDarkeningV2);//0.4*tempF2.z //darken

#endif //POSTPROCESS_V2



#ifdef APPLYGAMECOLORCORRECTION
	//apply original corrections (((
	r0.w=1.0;//_c81.y/r5.x;//r5 is AdapLumSampler

	luma=0.27*color.r + 0.67*color.g + 0.06*color.b;
	color.xyz=color.xyz * r0.w - luma;
	color.xyz=_c82.x * color.xyz + luma; //deSatContrastGamma.x
	r0.w=luma * _c84.w; //ColorShift.w
	r1.xyz=r0.w * _c84.xyz; //ColorShift

	//darken as DarkeningAmount, but keeps colors
	color.xyz=color.xyz - _c84.xyz * r0.w; //ColorShift
	color.xyz=saturate(r0.w) * color.xyz + r1.xyz;

	//coloring (green, brown, ...)
	color.xyz=2.0 * color.xyz * _c83.xyz; //ColorCorrect

	//desaturating
	luma=saturate(luma);
	r0.w=_c82.z - _c1.y; //deSatContrastGamma.z - 1.0
	color.xyz=color.xyz * pow(luma, r0.w);
#endif //APPLYGAMECOLORCORRECTION

_oC0.xyz=color.xyz;



	//_oC0.w=1.0;
	return _oC0;
}


//mine temporary post process
technique Shader_C215BE6E
{
    pass p0 
    {
		PixelShader  = compile ps_3_0 PS_C215BE6E();
	}
}




//original shader of post processing
technique Shader_ORIGINALPOSTPROCESS
{
    pass p0 
    {

	PixelShader= 
	asm
	{
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.23.949.2378
//
// Parameters:
//
//   sampler2D AdapLumSampler;
//   sampler2D BloomSampler;
//   float4 ColorCorrect;
//   float4 ColorShift;
//   float Exposure;
//   sampler2D GBufferTextureSampler2;
//   sampler2D GBufferTextureSampler3;
//   sampler2D HDRSampler;
//   sampler2D JitterSampler;
//   float PLAYER_MASK;
//   sampler2D StencilCopySampler;
//   float4 TexelSize;
//   float4 ToneMapParams;
//   float4 deSatContrastGamma;
//   float4 dofBlur;
//   float4 dofDist;
//   float4 dofProj;
//   float gDirectionalMotionBlurLength;
//   float4 globalScreenSize;
//   row_major float4x4 motionBlurMatrix;
//
//
// Registers:
//
//   Name                         Reg   Size
//   ---------------------------- ----- ----
//   globalScreenSize             c44      1
//   Exposure                     c66      1
//   motionBlurMatrix             c72      4
//   TexelSize                    c76      1
//   dofProj                      c77      1
//   dofDist                      c78      1
//   dofBlur                      c79      1
//   gDirectionalMotionBlurLength c80      1
//   ToneMapParams                c81      1
//   deSatContrastGamma           c82      1
//   ColorCorrect                 c83      1
//   ColorShift                   c84      1
//   PLAYER_MASK                  c85      1
//   GBufferTextureSampler2       s0       1
//   GBufferTextureSampler3       s1       1
//   HDRSampler                   s2       1
//   BloomSampler                 s3       1
//   AdapLumSampler               s4       1
//   JitterSampler                s5       1
//   StencilCopySampler           s6       1
//

    ps_3_0
    def c0, 0, 0.212500006, 0.715399981, 0.0720999986
    def c1, 0.25, 1, 256, 0
    def c2, 0, 2, 4, 8
    def c3, 58.1640015, 47.1300011, 0, 0
    def c4, -0.5, -1.5, 1.5, 0.5
    def c5, 2, -1, 0.125, 0
    def c6, 256, 2, 4, 8
def c180, 0.0, 0.0, 0.0, 100.5
def c181, 1.0, 0.0, 0.0, 20.01
def c183, 800.0, 600.0, 0.0, 100.5
    defi i0, 7, 0, 0, 0
    dcl_texcoord v0.xy
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_2d s3
    dcl_2d s4
    dcl_2d s5
    dcl_2d s6
    texld r0, v0, s1
    add r0.y, -c77.x, c77.y
    rcp r0.y, r0.y
    mul r0.z, r0.y, c77.y
    mul r0.z, r0.z, -c77.x
    mad r0.x, c77.y, -r0.y, r0.x
    rcp r0.x, r0.x
    mul r0.y, r0.z, r0.x
    texld r1, v0, s2
    texld r2, v0, s3
    abs r0.w, c79.w
    mov r3, c4
    mad r4.xy, c76, r3, v0
    texld r4, r4, s2
    mad r5.xy, c76, r3.zxzw, v0
    texld r5, r5, s2
    mad r3.xz, c76.xyyw, r3.wyzw, v0.xyyw
    texld r6, r3.xzzw, s2
    mad r3.xy, c76, r3.ywzw, v0
    texld r7, r3, s2
    dp3 r1.w, r4, r4
    rcp r1.w, r1.w
    cmp r3.xyz, -r1.w, c0.x, r4
    dp3 r1.w, r5, r5
    rcp r1.w, r1.w
    cmp r4.xyz, -r1.w, c0.x, r5
    dp3 r1.w, r6, r6
    rcp r1.w, r1.w
    cmp r5.xyz, -r1.w, c0.x, r6
    dp3 r1.w, r7, r7
    rcp r1.w, r1.w
    cmp r6.xyz, -r1.w, c0.x, r7
    dp3 r1.w, r1, r1
    rcp r1.w, r1.w
    cmp r7.xyz, -r1.w, c0.x, r1
    mad r0.x, r0.z, r0.x, -c78.w
    mad r0.x, c78.y, -r3.w, r0.x
    max r1.w, r0.x, c0.x
    rcp r0.x, c78.z
    mul r0.x, r1.w, r0.x
    lrp r1.w, r0.x, c79.z, c79.y
    min r0.x, c79.z, r1.w
    dp3 r0.z, r7, c0.yzww
    dp3 r8.x, r3, c0.yzww
    dp3 r8.y, r4, c0.yzww
    dp3 r8.z, r5, c0.yzww
    dp3 r8.w, r6, c0.yzww
    dp4 r1.w, r8, c1.x
    add r8, r8, -r1.w
    dp4 r2.w, r8, r8
    add r0.z, r0.z, -r1.w
    mad r0.z, r0.z, r0.z, -r2.w
    mul r0.x, r0.x, r0.x
    cmp r0.x, r0.z, c1.y, r0.x
    texld r8, v0, s0
    mov r8.yz, c1
    mad r9, r8.w, -r8.z, c2
    cmp r9, r9, c1.y, c1.w
    mad r10, r8.w, -c6.x, c6.yzwx
    cmp r10, r10, -c1.y, -c1.w
    add r9, r9, r10
    mul r9, r9, c4.x
    dp4 r0.z, r9, c1.y
    add r0.z, r0.z, c4.w
    rcp r0.z, r0.z
    mul r8.xzw, r3.xyyz, r9.x
    mad r7.xyz, r7, c4.w, r8.xzww
    mad r7.xyz, r4, r9.y, r7
    mad r7.xyz, r5, r9.z, r7
    mad r7.xyz, r6, r9.w, r7
    mul r7.xyz, r0.z, r7
    add r0.z, -r0.x, c1.y
    mul r1.w, r0.x, c1.x
    mul r3.xyz, r3, r1.w
    mad r3.xyz, r7, r0.z, r3
    mad r3.xyz, r4, r1.w, r3
    mad r3.xyz, r5, r1.w, r3
    mad r3.xyz, r6, r1.w, r3
    mad r4.xyz, v0.yxyw, c5.x, c5.y
    mul r0.z, r4.y, c77.z
    mul r0.z, r0.y, r0.z
    mul r1.w, -r4.x, c77.w
    mul r1.w, r0.y, r1.w
    mul r5.xyz, r1.w, c73
    mad r5.xyz, r0.z, c72, r5
    mad r5.xyz, -r0.y, c74, r5
    add r5.xyz, r5, c75
    mul r0.y, -r5.z, c77.z
    rcp r0.y, r0.y
    mul r6.x, r5.x, r0.y
    mul r0.y, r5.z, c77.w
    rcp r0.y, r0.y
    mul r6.y, r5.y, r0.y
    add r0.yz, -r4, r6.xxyw
    mul r0.yz, r0, c80.x
    mul r4.xy, r0.yzzw, c5.z
    texld r5, v0, s6
    add r1.w, r5.x, -c85.x
    mul r4.zw, r3.xyxy, c2.w
    mad r4.zw, v0.xyxy, c3.xyxy, r4
    texld r5, r4.zwzw, s5
    add r2.w, r5.x, c4.x
    mad r4.zw, r4.xyxy, r2.w, v0.xyxy
    mov r5.xyz, r3
    mov r2.w, c1.y
    mov r3.w, c1.y

    rep i0
      mad r6.xy, r4, r3.w, r4.zwzw
      texld r7, r6, s6
      add r5.w, r7.x, -c85.x
      cmp r5.w, r5.w, c1.w, c1.y
      texld r6, r6, s2
      mad r5.xyz, r6, r5.w, r5
      add r2.w, r2.w, r5.w
      add r3.w, r3.w, c1.y
    endrep

    rcp r2.w, r2.w
    mul r0.yz, r0, c183.xxyw//c44.xxyw
    dp2add r0.y, r0.yzzw, r0.yzzw, c0.x
    rsq r0.y, r0.y
    rcp r0.y, r0.y
    mul r0.y, r0.y, c4.w
    add r0.x, r0.x, c1.y
    rcp r0.x, r0.x
    mul_sat r0.x, r0.y, r0.x
    mad r4.xyz, r5, r2.w, -r3
    mad r0.xyz, r0.x, r4, r3
    cmp r0.xyz, r1.w, r3, r0
    cmp r0.xyz, -r0.w, r0, r1

    texld r1, c0.x, s4
    rcp r0.w, r1.x
    mul r0.w, r0.w, c81.y
    rcp r1.x, r0.w
    mul r1.x, r1.x, c81.x
    mad r1.xyz, r2, c66.x, -r1.x
    max r2.xyz, r1, c0.x
    mul r1.xyz, r2, c81.z
    mul r1.xyz, r1, c1.x
    mad r0.xyz, r0, c66.x, r1
    mul r1.xyz, r0.w, r0
    dp3 r1.x, r1, c0.yzww
    mad r0.xyz, r0, r0.w, -r1.x
    mad r0.xyz, c82.x, r0, r1.x
    mul r0.w, r1.x, c84.w
    mul r1.yzw, r0.w, c84.xxyz
    mov_sat r2.x, r0.w
    mad r0.xyz, c84, -r0.w, r0
    mad r0.xyz, r2.x, r0, r1.yzww
    mul r0.xyz, r0, c83
    add r0.xyz, r0, r0
    mov_sat r1.x, r1.x
    add r0.w, -r8.y, c82.z
    pow r2.x, r1.x, r0.w
//mul r2.x, r2.x, c181.x
    mul oC0.xyz, r0, r2.x
    mov oC0.w, c1.y

//texld r1, v0, s2
//mul oC0.xyz, r1, c199.z
	};
    }
}


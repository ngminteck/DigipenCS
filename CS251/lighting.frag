/////////////////////////////////////////////////////////////////////////
// Pixel shader for lighting
////////////////////////////////////////////////////////////////////////
#version 330



// These definitions agree with the ObjectIds enum in scene.h
const int     nullId	= 0;
const int     skyId	= 1;
const int     seaId	= 2;
const int     groundId	= 3;
const int     roomId	= 4;
const int     boxId	= 5;
const int     frameId	= 6;
const int     lPicId	= 7;
const int     rPicId	= 8;
const int     teapotId	= 9;
const int     spheresId	= 10;

in vec3 normalVec, lightVec ,eyeVec;
in vec2 texCoord;

uniform int objectId;
uniform vec3 diffuse ,specular , Light, Ambient ;
uniform float shininess;
uniform sampler2D tex;


void main()
{
    vec3 N = normalize(normalVec);
    vec3 L = normalize(lightVec);

	vec3 V = normalize(eyeVec);

    vec3 Kd = diffuse;   

	vec3 Ks = specular;
	float alpha = shininess;

	vec3 H = normalize(L+V);
    float LN = max(dot(L,N),0.0);
    float HN = max(dot(H,N),0.0);

    if (objectId==seaId)
	{
        ivec2 uv = ivec2(floor(200.0*texCoord));
        if ((uv[0]+uv[1])%2==0)
            Kd *= 0.9; 

	}
	else if(objectId==skyId)
	{
		vec2 uv = texCoord.xy;
		Kd = texture(tex, uv).xyz;
	}
	else if(objectId==groundId)
	{
		vec2 uv = texCoord.xy * 50;
		Kd = texture(tex, uv).xyz;
	}
	else if(objectId==roomId)
	{
		vec2 uv = vec2(texCoord.y * 20 ,texCoord.x * 10);
		Kd = texture(tex, uv).xyz;
	}
	else if(objectId==boxId)
	{
	    vec2 uv = texCoord.xy;
		Kd = texture(tex, uv).xyz;
	}
	else if(objectId==teapotId)
	{
		vec2 uv = texCoord.xy;
		Kd = texture(tex, uv).xyz;
	}
	else if(objectId==lPicId)
	{
		ivec2 uv = ivec2(floor(16.0*texCoord));
        if ((uv[0]+uv[1])%2==0)
            Kd = vec3(1.0,0.0,0.0);
		else
			Kd = vec3(0.0,0.0,1.0);
	}
    else if(objectId==rPicId)
	{
		float fx = fract(texCoord.x);
		float fy = fract(texCoord.y);

		for(float f =0.0 ; f <= 1.0; f+=0.1)
		{
		   if(fx < 0.1 || fx > 0.9 || fy < 0.1 || fy > 0.9 )
		   {
				Kd = vec3(0.5,0.5,0.5);
	       }
		   else
			{
				vec2 uv = vec2( texCoord.x * 1.25 - 0.125 ,texCoord.y *1.25 -0.125);
				Kd = texture(tex, uv).xyz;
				
			}
		}
	}
   

	vec3 F = Ks + (vec3(1,1,1)-Ks) * pow((1 -max(dot(L,H),0.0)),5.0);

	float G = 1/pow(max(dot(L,H),0.0),2.0);

	float D = (alpha + 2)/ (2 * 3.14159) * pow(max(dot(N,H),0.0),alpha);
	
	vec3 BRDF = (Kd / 3.14159) +  (F * G * D / 4) ;

	gl_FragColor.xyz = Ambient * Kd +  Light * max(dot(N,L),0.0) * BRDF;

	 //gl_FragColor.xyz = vec3(0.5,0.5,0.5)*Kd + Kd*max(dot(L,N),0.0);
}

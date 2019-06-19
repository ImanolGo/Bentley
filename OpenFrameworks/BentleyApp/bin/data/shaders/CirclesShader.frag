#version 120

uniform vec4 iColor = vec4(1.0,1.0,1.0, 1.0);
uniform vec3 iResolution;
uniform float iGlobalTime;
uniform float parameter = 1.0;


// This code can be found in 
// https://www.shadertoy.com/view/ldX3zr
// and it's property of its creator.
// This is distributed for illustration purposes only.

vec2 center = vec2(0.5,0.5);
float speed = 0.095;


void main()
{
	vec2 uv= gl_FragCoord.xy/iResolution.xy;
    float invAr = iResolution.y / iResolution.x;

    vec3 col = vec4(uv,0.5+0.5,1.0).xyz;
    vec3 texcol;

    float x = (center.x-uv.x);
    float y = (center.y-uv.y) *invAr;

    //float r = -sqrt(x*x + y*y); //uncoment this line to symmetric ripples
    float r = -0.5*(x*x + y*y);
    //float r = -(x*x*x + y*y*y);
    float z = 1.0 + 1.0*sin((r+iGlobalTime*speed)/0.013);

    texcol.x = z;
    texcol.y = z;
    texcol.z = z;
    texcol = 1 - texcol;
    gl_FragColor = vec4(iColor.rgb*texcol,iColor.a);
}
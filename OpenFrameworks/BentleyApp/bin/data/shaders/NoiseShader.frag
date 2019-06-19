#version 120

uniform vec4 iColor = vec4(1.0,1.0,1.0, 1.0);
uniform vec3 iResolution;
uniform float iGlobalTime;
uniform float parameter = 1.0;


 // This code can be found in 
// https://www.shadertoy.com/view/Ms3SWs
// and it's property of its creator.
// This is distributed for illustration purposes only.


float hash(vec2 p)
{
    vec3 p3  = fract(vec3(p.xyx) * 0.1031);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

float ang(vec2 uv, vec2 center){
    return atan((uv.y-center.y),(uv.x-center.x));
}

float spir(vec2 uv, vec2 loc){
    float dist1=length(uv-loc);
    float dist2=dist1*dist1;
    float layer6=sin((ang(uv,loc)+dist2-iGlobalTime)*6.0);
    layer6 = layer6*dist1;
    return layer6;
}

float ripl(vec2 uv, vec2 loc, float speed, float frequency){
    return sin(iGlobalTime*speed-length(uv-loc)*frequency);
}

float height(in vec2 uv){
    float layer1=sin(iGlobalTime*8.54-parameter*sin(length(uv-vec2(-0.41,-0.47)))*55.0);
    float layer2=sin(iGlobalTime*7.13-parameter*sin(length(uv-vec2(1.35,1.32)))*43.0);
    float layer3=sin(iGlobalTime*7.92-parameter*sin(length(uv-vec2(-0.34,1.28)))*42.5);
    float layer4=sin(iGlobalTime*6.71-parameter*sin(length(uv-vec2(1.23,-0.24)))*47.2);

    float spiral=spir(uv,vec2(0.7,0.3));
    spiral*=3.0;
    
    float temp = layer1+layer2+layer3+layer4+spiral;
    
    float b=smoothstep(-1.5,7.0,temp);
    return b*2.0;
}

void main()
{
	vec2 uv= gl_FragCoord.xy/iResolution.xy;
    float waveHeight=2.1*height(uv) - 0.1;            
    vec3 color=vec3(waveHeight*iColor.r,waveHeight*iColor.g,waveHeight*iColor.b);
    gl_FragColor = vec4(color, iColor.a );
}
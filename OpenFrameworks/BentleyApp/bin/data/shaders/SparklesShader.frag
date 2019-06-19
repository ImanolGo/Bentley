#version 120

uniform vec4 iColor = vec4(1.0,1.0,1.0, 1.0);
uniform vec3 iResolution;
uniform float iGlobalTime;
uniform float parameter = 1.0;
uniform int direction = 0;


// This code can be found in 
// https://www.shadertoy.com/view/MscXD7
// and it's property of its creator.
// This is distributed for illustration purposes only.


#define _SnowflakeAmount 100    // Number of snowflakes
#define _BlizardFactor 0.2      // Fury of the storm !
vec2 uv;

float rnd(float x)
{
    return fract(sin(dot(vec2(x+47.49,38.2467/(x+2.3)), vec2(12.9898, 78.233)))* (43758.5453));
}

float drawCircle(vec2 center, float radius)
{
    return 1.0 - smoothstep(0.0, radius, length(uv - center));
}


void main()
{
    // if(direction>1){
    //     uv = gl_FragCoord.xy / iResolution.xy;
    // }
    // else{
    //     uv = gl_FragCoord.yx / iResolution.xy;
    // }

     uv = gl_FragCoord.xy / iResolution.xy;

    //uv = gl_FragCoord.xy / iResolution.xy;

    vec4 color = vec4(0, 0, 0, 1.0);
    float j;
    for(int i=0; i<_SnowflakeAmount; i++)
    {
        j = float(i);
        float speed = 0.3+rnd(cos(j))*(0.7+0.5*cos(j/(float(_SnowflakeAmount)*0.25)));

        vec2 center;
       
        if(direction>1){
            center.x = (0.25-uv.y)*_BlizardFactor+rnd(j)+0.1*cos(iGlobalTime+sin(j));
            center.y = mod(sin(j)-speed*(iGlobalTime*1.5*(0.1+_BlizardFactor)), 1.0);
        }
        else{
            center.y = (0.25-uv.x)*_BlizardFactor+rnd(j)+0.1*cos(iGlobalTime+sin(j));
            center.x = mod(sin(j)-speed*(iGlobalTime*1.5*(0.1+_BlizardFactor)), 1.0);
        }
        
        if(mod(direction,2) == 0){
            center = 1-center;
        }



        color += (vec4(drawCircle(center, 0.001+speed*0.022))*iColor);
    }


    gl_FragColor = vec4(color.rgb, iColor.a );
}
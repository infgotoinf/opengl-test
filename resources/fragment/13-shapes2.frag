#version 330 core

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

#define PI 3.14159265359

float vec2float(in vec2 vec){
    return vec.x * vec.y;
}

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = vec3(0.0);

    // bottom-left
    float pct = vec2float(sin(step(vec2(0.1),cos(st+u_time*2))+u_time*2));

    // top-right
    pct *= vec2float(sin(step(vec2(0.1),cos(1.0-st-u_time*2))+u_time*2));


    color = vec3(pct);
    gl_FragColor = vec4(color,1.0);
}

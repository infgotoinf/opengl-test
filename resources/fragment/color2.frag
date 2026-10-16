#version 330 core

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

#define PI 3.14159265359

float plot(vec2 st, float pct){
  return  smoothstep( pct-0.02, pct, st.y) -
          smoothstep( pct, pct+0.02, st.y);
}

vec3 colorA = vec3(1,0,0.5);
vec3 colorB = vec3(0,0,1);

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution;

    vec3 color = vec3(0);
    float pct = sin(mod(u_time, 1));

    color = mix(colorA, colorB, pct);

    gl_FragColor = vec4(color,1.0);
}

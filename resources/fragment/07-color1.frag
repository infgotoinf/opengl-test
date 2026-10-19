#version 330 core

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

#define PI 3.14159265359

float plot(vec2 st, float pct){
  return  smoothstep( pct-0.02, pct, st.y) -
          smoothstep( pct, pct+0.02, st.y);
}

vec3 colorA = vec3(0.149,0.141,0.912);
vec3 colorB = vec3(1.000,0.833,0.224);

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution;

    vec3 color = vec3(0);
    color.rg = vec2(sin(st.x * PI + u_time * 10));

    color.rgb = color.brb;

    gl_FragColor = vec4(color,1.0);
}

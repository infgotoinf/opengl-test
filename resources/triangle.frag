#version 330 core

uniform vec2 iResolution;
uniform float iTime;

void main() {
  vec2 st = gl_FragCoord.xy / iResolution.xy;
  float t = iTime * 0.5;
  float v = sin(st.x * 10.0 + t)
          + sin(st.y * 10.0 + t)
          + sin((st.x + st.y) * 8.0 + t * 1.3);
  vec3 col = 0.5 + 0.5 * cos(vec3(0.0, 2.0, 4.0) + v + t);
  gl_FragColor = vec4(col, 1.0);
}

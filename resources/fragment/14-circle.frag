#version 330 core

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

#define PI 3.14159265359

void main(){
	vec2 st = gl_FragCoord.xy/u_resolution;
    float pct = 0.0;

    // a. The DISTANCE from the pixel to the center
    pct = distance(st,vec2(0.5));
    pct = step(0.5, pct);
    // pct = smoothstep(0.4, 0.6, pct);

    // pct = distance(st,vec2(0.4)) + distance(st,vec2(0.6));
    // pct = distance(st,vec2(0.4)) * distance(st,vec2(0.6));
    // pct = min(distance(st,vec2(0.4)),distance(st,vec2(0.6)));
    // pct = max(distance(st,vec2(0.4)),distance(st,vec2(0.6)));
    // pct = pow(distance(st,vec2(0.4)),distance(st,vec2(0.6)));

    // b. The LENGTH of the vector
    //    from the pixel to the center
    // vec2 toCenter = vec2(0.5)-st;
    // pct = length(toCenter);

    // c. The SQUARE ROOT of the vector
    //    from the pixel to the center
    // vec2 tC = vec2(0.5)-st;
    // pct = sqrt(tC.x*tC.x+tC.y*tC.y);

    vec3 color = vec3(pct);
    color.r = abs(sin(color.r+distance(st,vec2(0.5))+u_time+sin(st.x/4+u_time)+sin(st.y/2+u_time)));

    color.g = abs(sin(color.g+distance(st,vec2(0.5))+u_time-sin(st.x/4+u_time)-sin(st.y/4+u_time)));

    color.b = abs(sin(color.b+distance(st,vec2(0.5))/2+u_time*2));

    color *= 1-pct;
    color = 1-color;

	gl_FragColor = vec4( color, 1.0 );
}

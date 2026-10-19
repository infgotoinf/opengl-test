#version 330 core

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

#define PI 3.14159265359


float circle(in vec2 _st, in float _radius){
    vec2 l = _st-vec2(0.5);
    return 1.-smoothstep(_radius-(_radius*0.01),
                         _radius+(_radius*0.01),
                         dot(l,l)*4.0);
}

const int ADD       = 0;
const int SUBSTRACT = 1;

void patternize(inout vec2 st, inout vec3 color, in float modif, in int operation)
{
    st *= modif;    // Scale up the space by modif
    st = fract(st); // Wrap around 1.0
    color += vec3(st,0.);
    switch (operation)
    {
    case SUBSTRACT:
        color -= vec3(circle(st,1.));
        break;
    case ADD:
        color += vec3(circle(st,1.));
        break;
    }
}

mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution;
    vec3 color = vec3(0.0);

    float modif = max(0., 100-fract(+u_time*0.2)*100);

    st -= st/modif;
    st -= 0.5;
    st *= rotate2d( u_time );
    patternize(st, color, modif, ADD);
    patternize(st, color, modif, ADD);
    patternize(st, color, modif, SUBSTRACT);
    patternize(st, color, modif, SUBSTRACT);
    // patternize(st, color, modif, SUBSTRACT);
    // patternize(st, color, modif, SUBSTRACT);
    // patternize(st, color, modif, SUBSTRACT);
    // patternize(st, color, modif, SUBSTRACT);

    // st *= modif;    // Scale up the space by 3
    // st = fract(st); // Wrap around 1.0
    // color = vec3(st,0.);
    // color += vec3(circle(st,0.5));

    // st *= modif;
    // st = fract(st);
    // color += vec3(st,0.);
    // color -= vec3(circle(st,0.5));

    // st *= modif;
    // st = fract(st);
    // color += vec3(st,0.);
    // color -= vec3(circle(st,0.5));

    // st *= modif;
    // st = fract(st);
    // color += vec3(st,0.);
    // color -= vec3(circle(st,0.5));

    gl_FragColor = vec4(color,1.);
}

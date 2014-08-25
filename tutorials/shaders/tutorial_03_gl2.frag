#version 110

varying vec3 g_normal;
varying vec4 g_object_position;

uniform int g_inner_surface;
uniform bool g_wireframe;

void main(void)
{
    vec3 light_direction = normalize(vec3(1.0, 0.5, 0.6));

    if (!g_wireframe)
    {
        if (g_inner_surface == 1)
            gl_FragColor = vec4(vec3(1.0, 1.0, 1.0) * max(0.5, dot(light_direction, g_normal)), 1.0);
        else
            gl_FragColor = vec4(vec3(1.0, 0.0, 0.0) * max(0.5, dot(light_direction, g_normal)), 0.3);
    }
    else
    {
        if (g_inner_surface == 1)
            gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        else
            gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
}


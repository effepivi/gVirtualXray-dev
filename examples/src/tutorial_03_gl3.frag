#version 150
precision highp float;

smooth in vec3 g_normal;
smooth in vec4 g_object_position;

out vec4 fragColor;

uniform int g_inner_surface;
uniform bool g_wireframe;

void main(void)
{
    vec3 light_direction = normalize(vec3(1.0, 0.5, 0.6));

    if (!g_wireframe)
    {
        if (g_inner_surface == 1)
            fragColor = vec4(vec3(1.0, 1.0, 1.0) * max(0.5, dot(light_direction, g_normal)), 1.0);
        else
            fragColor = vec4(vec3(1.0, 0.0, 0.0) * max(0.5, dot(light_direction, g_normal)), 0.3);
    }
    else
    {
        if (g_inner_surface == 1)
            fragColor = vec4(1.0, 1.0, 1.0, 1.0);
        else
            fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
}


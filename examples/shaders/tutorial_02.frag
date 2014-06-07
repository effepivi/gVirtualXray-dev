#version 150
precision highp float;

smooth in vec3 g_normal;
smooth in vec4 g_object_position;

out vec4 fragColor;
void main(void)
{
    vec3 light_direction = normalize(vec3(1.0, 0.5, 0.6));
    fragColor = vec4(vec3(0.0, 0.0, 1.0) * max(0.0, dot(light_direction, g_normal)), 1.0);
}

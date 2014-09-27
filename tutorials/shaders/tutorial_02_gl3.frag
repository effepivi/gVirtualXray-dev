#version 150
precision highp float;

smooth in vec3 g_vertex_color;

out vec4 fragColor;

void main(void)
{
    fragColor = vec4(g_vertex_color, 1.0);
}

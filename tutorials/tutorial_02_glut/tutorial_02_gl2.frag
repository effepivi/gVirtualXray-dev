#version 110

varying vec3 g_vertex_color;

void main(void)
{
    gl_FragColor = vec4(g_vertex_color, 1.0);
}

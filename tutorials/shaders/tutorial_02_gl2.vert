#version 110

varying vec3 g_vertex_color;

uniform mat4 g_projection_matrix;
uniform mat4 g_modelview_matrix;

void main(void)
{
    g_vertex_color = gl_Vertex.xyz / length(gl_Vertex.xyz);
    gl_Position = g_projection_matrix * g_modelview_matrix * vec4(gl_Vertex.xyz, 1.0);
}

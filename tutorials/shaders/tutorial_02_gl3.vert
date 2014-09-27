#version 150

in vec3 in_Vertex;

smooth out vec3 g_vertex_color;

uniform mat4 g_projection_matrix;
uniform mat4 g_modelview_matrix;

void main(void)
{
    g_vertex_color = in_Vertex / length(in_Vertex);
    gl_Position = g_projection_matrix * g_modelview_matrix * vec4(in_Vertex, 1.0);
}

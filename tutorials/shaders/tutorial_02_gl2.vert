#version 110

uniform mat4 g_projection_matrix;
uniform mat4 g_modelview_matrix;

void main(void)
{
    gl_Position = g_projection_matrix * g_modelview_matrix * vec4(gl_Vertex.xyz, 1.0);
}


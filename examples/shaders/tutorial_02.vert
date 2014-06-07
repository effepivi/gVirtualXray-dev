#version 150

in vec3 in_Vertex;
in vec3 in_TexCoord;
in vec3 in_Normal;

smooth out vec3 g_normal;
smooth out vec4 g_object_position;

uniform mat4 g_projection_matrix;
uniform mat4 g_modelview_matrix;
uniform mat3 g_normal_matrix;

void main(void)
{
	g_object_position = g_modelview_matrix * vec4(in_Vertex, 1.0);
	g_normal = normalize(g_normal_matrix * in_Normal);
	gl_Position = g_projection_matrix * g_object_position;
}

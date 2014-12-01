/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


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


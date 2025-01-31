#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos; // cubemap se dokáže obejít bez texCoords a získat je z pozice -> (0,0,0);
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // z = w (1.0), aby depth value byl vždy 1.f
}  
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform vec4 color;
void main()
{
  if (color.a < 0.1)
    FragColor = texture(texture1, TexCoord);
  else
    FragColor = color;
}


#version 460 core

in vec3 TexCoords; // direction vector representing a 3D texture coordinate
uniform samplerCube cubemap;

out vec4 FragColor;

void main()
{
    FragColor = texture(cubemap, TexCoords);
}
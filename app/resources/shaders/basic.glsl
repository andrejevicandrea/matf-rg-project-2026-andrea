//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos; //ulazni atribut za poziciju
layout (location = 1) in vec2 aTexCoords; // ulazni atribut za poziciju teksture

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}

//#shader fragment
#version 330 core

in vec2 TexCoords;

out vec4 FragColor; //izlazni atribut boja

uniform vec3 objectColor;
uniform bool useTexture;
uniform sampler2D texture_diffuse;

void main() {
    if (useTexture) {
        FragColor = texture(texture_diffuse, TexCoords);

    } else {
        FragColor = vec4(objectColor, 1.0f);
    }
}
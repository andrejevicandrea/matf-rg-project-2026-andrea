//#shader vertex
#version 330 core

/*
prethodni shader:
layout (location = 0) in vec3 aPos; //ulazni atribut za poziciju
layout (location = 1) in vec2 aTexCoords; // ulazni atribut za poziciju teksture
mora da se menja sa dolaskom modela jer se ubacuje i aNormal ali na location 1

*/
layout (location = 0) in vec3 aPos; //ulazni atribut za poziciju
layout (location = 1) in vec3 aNormal; // ulazni atribut za normale
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);

}

/* promena i kod fragment shadera, posebni sampleri za teksturu i za model
*/

//#shader fragment
#version 330 core

in vec2 TexCoords;

out vec4 FragColor; //izlazni atribut boja


uniform sampler2D texture_diffuse1; // za modele



void main() {
    FragColor = vec4(texture(texture_diffuse1, TexCoords).rgb, 1.0);
}
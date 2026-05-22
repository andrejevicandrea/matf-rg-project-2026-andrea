//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos; //ulazni atribut za poziciju

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor; //izlazni atribut boja

void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0); //postavljanje na belu boju samo radi testiranja, posle dolazi tekstura
}
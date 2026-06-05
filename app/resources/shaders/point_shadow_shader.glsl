//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;

void main() {
    gl_Position = model * vec4(aPos, 1.0);
}

//#shader geometry
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 shadow_matrices[6];

out vec4 frag_pos;

void main() {
    for (int face = 0; face < 6; face++) {
        gl_Layer = face;

        for (int i = 0; i < 3; i++) {
            frag_pos = gl_in[i].gl_Position;
            gl_Position = shadow_matrices[face] * frag_pos;
            EmitVertex();
        }

        EndPrimitive();
    }
}

//#shader fragment
#version 330 core

in vec4 frag_pos;

uniform vec3 light_position;
uniform float far_plane;

void main() {
    float light_distance = length(frag_pos.xyz - light_position);
    light_distance /= far_plane;


    gl_FragDepth = light_distance;

    
}
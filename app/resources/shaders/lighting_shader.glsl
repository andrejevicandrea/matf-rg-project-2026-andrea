//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);

}

//#shader fragment
#version 330 core

struct Material {
    sampler2D diffuse;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;

vec3 calculate_dir_light(DirLight light, vec3 normal, vec3 view_dir);
vec3 calculate_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main() {
    vec3 normal = normalize(Normal);
    vec3 view_dir = normalize(viewPos - FragPos);

    vec3 result = calculate_dir_light(dirLight, normal, view_dir);
    result += calculate_point_light(pointLight, normal, FragPos, view_dir);

    FragColor = vec4(result, 1.0);
}

vec3 calculate_dir_light(DirLight light, vec3 normal, vec3 view_dir) {
    vec3 texture_color = texture(material.diffuse, TexCoords).rgb;
    vec3 light_dir = normalize(-light.direction);

    //za diffuse
    float diff = max(dot(normal, light_dir), 0.0);

    //za specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    vec3 ambient = light.ambient * texture_color;
    vec3 diffuse = light.diffuse * diff * texture_color;
    vec3 specular = light.specular * spec;

    return ambient + diffuse + specular;
}

vec3 calculate_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir) {
    vec3 texture_color = texture(material.diffuse, TexCoords).rgb;
    vec3 light_dir = normalize(light.position - frag_pos);

    //za diffuse
    float diff = max(dot(normal, light_dir), 0.0);

    //za specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    //jednacina za attenuation
    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * texture_color;
    vec3 diffuse = light.diffuse * diff * texture_color;
    vec3 specular = light.specular * spec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;


}


#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec3 TangentLightPos;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;
uniform PointLight pointLight;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    TangentLightPos = TBN * pointLight.position;
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * FragPos; 

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
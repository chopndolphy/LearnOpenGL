#version 330 core
out vec4 FragColor;

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
in vec2 TexCoord;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec3 TangentLightPos;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform PointLight pointLight;
uniform DirLight dirLight;
uniform float shininess;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 norm = vec3(texture(texture_normal1, TexCoord));
    norm = normalize(norm * 2.0 - 1.0);

    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);

    vec3 color = vec3(texture(texture_diffuse1, TexCoord));
    
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    result += CalcPointLight(pointLight, norm, FragPos, viewDir, color);

    FragColor = vec4(result, 1.0);
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    float distance = length(TangentLightPos - TangentFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diff * color;
    vec3 specular = light.specular * spec * color;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoord));
    return (ambient + diffuse + specular);
}
#version 330 core
out vec4 FragColor;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
in vec2 TexCoord;

uniform sampler2D planeTexture;

void main()
{
    FragColor = texture(planeTexture, TexCoord);
}
// vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
// {
//     vec3 lightDir = normalize(-light.direction);

//     float diff = max(dot(normal, lightDir), 0.0);

//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
//     return (ambient + diffuse + specular);
// }
#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in mat3 TBN;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D specular;
uniform bool hasSpecularTexture;

uniform sampler2D normalMap;
uniform bool hasNormalMap;

uniform vec4 diffuseColor;
uniform float intensityMultiplier;
uniform vec3 lightPos;
uniform vec3 viewPos; 
uniform float shininess;

void main()
{
    vec3 normal = normalize(Normal);
    if (hasNormalMap) {
       vec3 tangentNormal = normalize((texture(normalMap, TexCoord).xyz * 2.0 ) - 1.0);
       normal = TBN * tangentNormal; 
    }

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfDir), 0.0), shininess); 

    vec3 ambientComponent;
    vec3 diffuseComponent;
    vec3 specularComponent;

    if (diffuseColor.a > 0.0) {
        // Use the diffuseColor uniform for coloring
        ambientComponent = 0.1 * vec3(diffuseColor);
        diffuseComponent = diff * vec3(diffuseColor);
    } else {
        // Use the diffuse texture for coloring
        ambientComponent = 0.1 * vec3(texture(diffuseTexture, TexCoord));
        diffuseComponent = diff * vec3(texture(diffuseTexture, TexCoord));
    }

    if (hasSpecularTexture) {
        specularComponent = spec * vec3(texture(specular, TexCoord)); // Specular component with texture
    } else {
        specularComponent = spec * vec3(1.0); // Specular component without texture
    }

    vec3 result = (ambientComponent + diffuseComponent + specularComponent) * intensityMultiplier;

    FragColor = vec4(result, 1.0);
}

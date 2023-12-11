#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec3 o_tangent;
in vec2 o_uv0;
out vec4 FragColor;

uniform vec4 color;

uniform sampler2D colorTexture;

uniform sampler2D normalMap;

uniform sampler2D metalRoughnessMap;

uniform sampler2D aoMap;

uniform sampler2D emissiveMap;

uniform samplerCube skybox;

uniform vec3 viewPosition;

const vec3 lightPos = vec3(-3.0, -3.0, -3.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const vec3 ambientColor = vec3(0.15, 0.15, 0.15);
const vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
const vec3 specularColor = vec3(1.0, 1.0, 1.0);
const float shininess = 16;

void main() {
    // Sample textures
    vec3 textureColor = (texture(colorTexture, o_uv0) * color).xyz;
    vec3 normal = texture(normalMap, o_uv0).xyz * 2.0 - 1.0;

    // Tangent space basis
    vec3 tangent = o_tangent;
    vec3 bitangent = cross(o_normalWorld, tangent);

    mat3 TBN = mat3(tangent, bitangent, o_normalWorld);

    // Transform normal to world space
    vec3 newNormal = TBN * normal;

    // Phong lighting model

    // Ambient
    vec3 ambient = ambientColor;

    // Diffuse
    vec3 lightDir = normalize(lightPos - o_positionWorld);
    float diffuseIntensity = max(dot(newNormal, lightDir), 0.0);
    vec3 diffuse = diffuseIntensity * diffuseColor;

    // Specular
    vec3 viewDir = normalize(viewPosition - o_positionWorld);
    vec3 reflectDir = reflect(-lightDir, newNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * specularColor;

    // Combine lighting components
    vec3 finalColor = textureColor * lightColor * (ambient + diffuse + specular);
    //FragColor = vec4(finalColor, 1.0);
    vec3 I = normalize(o_positionWorld - viewPosition);
    vec3 reflection = -reflect(I, normalize(newNormal));
    vec3 reflectedColor = texture(skybox, reflection).xyz;

    vec3 emissive = texture(emissiveMap, o_uv0).xyz;

    float metal = texture(metalRoughnessMap, o_uv0).z;

    float roughness = 0.6 + texture(metalRoughnessMap, o_uv0).y * 0.4;

    float ao = texture(aoMap, o_uv0).x;

    vec3 res1 = reflectedColor;

    vec3 res2 = textureColor * lightColor * (ambient + diffuse + metal * specular) * ao;

    vec3 result = emissive + (1.0 - roughness) * res1 + roughness * res2;

    FragColor = vec4(result, 1.0);

}
#version 450 core

in vec3 vWorldPosition;
in vec3 vNormal;
in vec2 vTexCoord;

layout(location = 0) out vec4 FragColor;

uniform vec3 uCameraPos;
uniform vec3 uLightDirection;
uniform vec3 uLightColor;
uniform vec3 uAmbientColor;

uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform float uShininess;
uniform float uOpacity;

uniform int uHasDiffuseMap;
uniform sampler2D uDiffuseMap;

void main() {
    const vec3 normal = normalize(vNormal);
    const vec3 lightDirection = normalize(uLightDirection);
    const vec3 viewDirection = normalize(uCameraPos - vWorldPosition);
    const vec3 halfway = normalize(lightDirection + viewDirection);

    const vec4 sampled = uHasDiffuseMap != 0 ? texture(uDiffuseMap, vTexCoord) : vec4(1.0);
    const vec3 diffuseColor = uDiffuseColor * sampled.rgb;

    const float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    const float specularFactor = pow(max(dot(normal, halfway), 0.0), max(uShininess, 1.0));

    const vec3 color = uAmbientColor * diffuseColor
        + uLightColor * (diffuseFactor * diffuseColor + specularFactor * uSpecularColor);

    FragColor = vec4(color, uOpacity * sampled.a);
}

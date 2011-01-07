#version 330

uniform vec3 baseColor;
uniform sampler2D textureData;

in vec3 transformedNormal;
in vec3 lightDirection;

in vec2 varyingTextureCoordinates;

out vec4 fragColor;

void main() {
    /* Light intensity */
    float intensity = max(0.0, dot(transformedNormal, lightDirection));

    vec4 textureColor = texture(textureData, varyingTextureCoordinates);

    /* Complete color */
    fragColor.rgb = (textureColor.rgb*textureColor.a + baseColor*(1-textureColor.a))*intensity;
    fragColor.a = 1.0;
}

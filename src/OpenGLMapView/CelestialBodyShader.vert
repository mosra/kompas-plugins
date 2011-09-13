#version 330

uniform mat4 transformMatrix;
uniform mat4 transformProjectionMatrix;
uniform mat3 normalMatrix;

uniform vec3 light;

in vec4 vertex;
in vec2 textureCoordinates;

out vec3 transformedNormal;
out vec3 lightDirection;
out vec2 varyingTextureCoordinates;

void main() {
    /* Transformed vertex position */
    vec4 transformedVertex4 = transformMatrix*vertex;
    vec3 transformedVertex = transformedVertex4.xyz/transformedVertex4.w;

    /* Transformed normal vector */
    transformedNormal = normalize(normalMatrix*vertex.xyz);

    /* Direction to the light */
    lightDirection = normalize(light - transformedVertex);

    varyingTextureCoordinates = textureCoordinates;

    gl_Position = transformProjectionMatrix*vertex;
}

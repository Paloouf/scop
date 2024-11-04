#version 410 core
out vec4 FragColor;
  
flat in vec3 vertexColor; // the input variable from the vertex shader (same name and same type)  
in vec2 texCoord;

uniform vec3 Kd;
uniform sampler2D texture1;
uniform bool useTexture;

void main()
{
    vec3 baseColor = vertexColor * Kd;

    if (useTexture) {
        // Sample texture and blend with the base color
        vec4 texColor = texture(texture1, texCoord);
        FragColor =  texColor; //vec4(baseColor, 1.0) * Combine base color with texture
    } else {
        // Use only the base color without texture
        FragColor = vec4(baseColor, 1.0);
    }
} 
#version 330
in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 resolution; // texture size
uniform vec2 direction; // vec2(1.0, 0.0) - horizontal, vec(0.0, 1.0) - vertical
uniform float blurRadius;
uniform float threshold;

void main() {
    vec4 color = vec4(0.0);
    vec2 tex_offset = direction / resolution * blurRadius; // step in pixels
    float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

    color += texture(texture0, fragTexCoord) * weight[0];

    for (int i=0; i<5; i++){
        color += texture(texture0, fragTexCoord + tex_offset * float(i)) * weight[i];
        color += texture(texture0, fragTexCoord - tex_offset * float(i)) * weight[i];
    }
    finalColor = color;
}
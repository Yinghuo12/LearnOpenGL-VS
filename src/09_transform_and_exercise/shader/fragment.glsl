#version 330 core

in vec3 ourColor;
in vec3 ourPos;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  float xy = length(ourPos.xy);
  FragColor = mix(texture(texture1, TexCoord), vec4(ourColor, 1.0 - xy * 2.0) * texture(texture2, TexCoord), 0.1);
}
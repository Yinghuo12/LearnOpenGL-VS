#version 330 core

in vec2 outTexCoord;
out vec4 FragColor;
uniform vec3 lightColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  FragColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);
  // FragColor = vec4(vec3(1.0f, 0.5f, 0.31f) * lightColor, 1.0);    //光照颜色为绿色(0,1,0)，而物体颜色为珊瑚红
}
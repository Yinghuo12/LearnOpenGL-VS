#version 330 core

in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;            // 观察者的位置(这里使用相机位置)
uniform float ambientStrength;   // 环境光强度

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {

  // vec4 objectColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);
  vec4 objectColor = vec4(1.0f, 0.5f, 0.31f, 1.0f);  // 珊瑚红


  // 光照单位向量与面片法线单位向量
  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(lightPos - outFragPos);

  // 环境光------ 
  vec3 ambient = ambientStrength * lightColor;

  // 漫反射------
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  // 镜面光------
  float specularStrength = 0.9;   // 镜面光强
  vec3 viewDir = normalize(viewPos - outFragPos);   // 得到面片指向观察者的向量，即视线向量
  vec3 reflectDir = reflect(-lightDir, norm);    // 得到反射向量 (第一个参数是从光源指向片段位置的向量)

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);  // 二者夹角越小，镜面光越强
  vec3 specular = specularStrength * spec * lightColor; // 镜面光

  vec3 result = (ambient + diffuse + specular) * vec3(objectColor);

  FragColor = vec4(result, 1.0);
}
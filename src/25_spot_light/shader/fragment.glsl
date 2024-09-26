#version 330 core

// 材质
struct Material {
  sampler2D diffuse;  // 漫反射贴图
  sampler2D specular; // 镜面光贴图
  float shininess;    // 高光指数
};


// 光源
struct Light {
  vec3 position; // 光源位置

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  /* 聚光 */
  vec3 direction;    // 光照方向
  float cutOff;      // 切光角(注意,为了优化计算,我们传入的是余弦值)
  float outerCutOff; // 外切光角(注意,为了优化计算,我们传入的是余弦值)

  /* 点光源 */
  float constant;      // 常数项
  float linear;        // 一次项
  float quadratic;     // 二次项
};


in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;
out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {

  // 物体颜色及纹理
  vec4 objectColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  vec3 diffuseTexture = vec3(texture(material.diffuse, outTexCoord));
  vec3 specularTexture = vec3(texture(material.specular, outTexCoord));

  // 环境光 -----
  vec3 ambient = light.ambient * diffuseTexture; 

  // 漫反射 -----
  vec3 lightDir = normalize(light.position - outFragPos);   // 面片指向光源的向量
  vec3 norm = normalize(outNormal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * diffuseTexture;

  // 镜面反射 -----
  vec3 viewDir = normalize(viewPos - outFragPos);   // 面片指向摄像机(人眼)的向量
  vec3 reflectDir = reflect(-lightDir, norm);    // 镜面反射向量
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * specularTexture;


  // 聚光软化边缘 ----------------------------
  float theta = dot(lightDir, normalize(-light.direction));   // 在聚光内部的话θ值应该比切光角小
  float epsilon = light.cutOff - light.outerCutOff;                   // 内外圆锥的余弦值差,用于软化圆锥边缘
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  // 约束强度在0-1之间
  diffuse *= intensity;
  specular *= intensity;
  // ----------------------------------


  // 点光源衰减 ----------------------
  float distance = length(light.position - outFragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0));
  // ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  // ----------------------------------


  vec3 result = (ambient + diffuse + specular) * vec3(objectColor);
  FragColor = vec4(result, 1.0);

}
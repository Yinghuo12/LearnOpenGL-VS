#version 330 core
in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

out vec4 FragColor;


// 材质
struct Material {
  sampler2D diffuse; // 漫反射贴图
  sampler2D specular; // 镜面光贴图
  float shininess; // 高光指数
};


// 光源
struct Light {
  vec3 position; // 光源位置

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  // 点光源衰减系数
  float constant;   // 常数项(一般都是1)
  float linear;     // 一次项
  float quadratic;  // 二次项
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos; // 相机位置

void main() {

  // 物体颜色及纹理
  vec4 objectColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  vec3 diffuseTexture = vec3(texture(material.diffuse, outTexCoord));
  vec3 specularTexture = vec3(texture(material.specular, outTexCoord));

  // 计算衰减值
  float distance = length(light.position - outFragPos);   // 计算片段与光源之间的距离
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0));  // 衰减值

  // 用于计算光照的向量
  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(light.position - outFragPos);  // 点光源
  // vec3 lightDir = normalize(light.position);  // 平行光
  vec3 viewDir = normalize(viewPos - outFragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  // 环境光 -----
  vec3 ambient = light.ambient * diffuseTexture;

  // 漫反射 -----
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * diffuseTexture; 

  // 镜面光 -----
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * specularTexture;

  // 将环境光、漫反射、镜面光分别乘以衰减距离
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  vec3 result = (ambient + diffuse + specular) * vec3(objectColor);

  FragColor = vec4(result, 1.0);
}
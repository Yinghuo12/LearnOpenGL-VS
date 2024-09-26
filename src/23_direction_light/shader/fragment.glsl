#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

// 定义材质结构体
struct Material {
  sampler2D diffuse; // 漫反射贴图
  sampler2D specular; // 镜面光贴图
  sampler2D specularColor; // 镜面光贴图
  sampler2D emission; // 发光贴图
  float shininess; // 高光指数
};

// 光源属性
struct Light {
  vec3 direction; // 定向光(平行光)
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos; // 观察位置即相机位置

void main() {
  // 物体颜色及纹理信息
  vec4 objectColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  vec3 diffuseTexture = vec3(texture(material.diffuse, outTexCoord));
  vec3 specularTexture = vec3(texture(material.specular, outTexCoord));

  // 变量
  vec3 norm = normalize(outNormal);
  // vec3 lightDir = normalize(lightPos - outFragPos);
  vec3 lightDir = normalize(light.direction);  // 平行光
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

  vec3 result = (ambient + diffuse + specular) * vec3(objectColor);

  FragColor = vec4(result, 1.0);
}
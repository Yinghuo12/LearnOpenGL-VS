#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos; // 相机位置

// 定义材质结构体
struct Material {
  sampler2D diffuse;    // 漫反射贴图  这里对比20节，用采样器替代了vec3颜色属性
  sampler2D specular;   // 镜面光贴图
  float shininess;      // 高光指数
};

// 光源属性
struct Light {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Material material;
uniform Light light;

void main() {

  vec4 objectColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  // 获取漫反射贴图和镜面光贴图
  vec3 diffuseTexture = vec3(texture(material.diffuse, outTexCoord));
  vec3 specularTexture = vec3(texture(material.specular, outTexCoord));

  // 光照和法线向量
  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(lightPos - outFragPos);

  // 环境光 -----
  vec3 ambient = light.ambient * diffuseTexture; 

  // 漫反射 -----
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * diffuseTexture; 

  // 镜面光 -----
  vec3 viewDir = normalize(viewPos - outFragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * specularTexture; // 镜面光贴图（木头材质应该没有镜面高光，而钢制边框受到镜面高光的影响）

  // vec3 specular = light.specular * spec; // 镜面光（不采用贴图）
  vec3 result = (ambient + diffuse + specular) * vec3(objectColor);

  FragColor = vec4(result, 1.0);
}
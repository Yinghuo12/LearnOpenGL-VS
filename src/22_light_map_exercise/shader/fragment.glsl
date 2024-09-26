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
  sampler2D diffuse;        // 漫反射贴图
  sampler2D specular;       // 镜面光贴图
  sampler2D specularColor;  // 彩色镜面光贴图
  sampler2D emission;       // 放射光贴图
  float shininess;          // 高光指数
};

// 光源属性
struct Light {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Material material;
uniform Light light;
uniform float factor;

void main() {

  // 物体颜色
  vec4 objectColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

  // 放射光变量(用于放射光贴图)
  vec2 uv = outTexCoord;
  uv.y += factor;

  // 三种贴图
  vec3 diffuseTexture = vec3(texture(material.diffuse, outTexCoord));         // 漫反射贴图
  vec3 specularTexture = vec3(texture(material.specularColor, outTexCoord));  // 彩色镜面光贴图(与下面二选一)
  // vec3 specularTexture = vec3(vec4(1.0) - texture(material.specular, outTexCoord));    // 实现木头与钢铁反光效果互换(镜面光贴图)
  vec3 emissionTexture = vec3(texture(material.emission, uv));                // 放射光贴图

  // 用于计算镜面光照的变量
  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(lightPos - outFragPos);
  vec3 viewDir = normalize(viewPos - outFragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

 
  // 环境光 ------
  vec3 ambient = light.ambient * diffuseTexture; 

  // 漫反射 ------
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * diffuseTexture;

  // 镜面光 ------
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * specularTexture;

  vec3 result = (ambient + diffuse + specular + emissionTexture) * vec3(objectColor);
  FragColor = vec4(result, 1.0);

}
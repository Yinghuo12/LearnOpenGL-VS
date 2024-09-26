#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <geometry/BoxGeometry.h>
#include <geometry/PlaneGeometry.h>
#include <geometry/SphereGeometry.h>

#include <shader.h>
#include <camera.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <gui.h>

#include <mesh.h>
#include <model.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const *path);


int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
// int SCREEN_WIDTH = 1600;
// int SCREEN_HEIGHT = 1200;

// camera value
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera(glm::vec3(0.0, 0.0, 5.0));

// delta time
float deltaTime = 0.0f;
float lastTime = 0.0f;

float lastX = SCREEN_WIDTH / 2.0f; // 鼠标上一帧的位置
float lastY = SCREEN_HEIGHT / 2.0f;


int main(int argc, char *argv[]) {
  glfwInit();
  // glfwWindowHint(GLFW_SAMPLES, 4);  // 片段着色器将作用域每一个采样点（采用4倍抗锯齿，则每个像素有4个片段（四个采样点））
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 窗口对象
  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // imgui -----------------------
  const char *glsl_version = "#version 330";
  ImGui::CreateContext(); // 创建imgui上下文
  ImGuiIO &io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark(); // 设置样式
  ImGui_ImplGlfw_InitForOpenGL(window, true); // 设置平台和渲染器
  ImGui_ImplOpenGL3_Init(glsl_version);
  // -----------------------

  // 设置视口
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LESS);

  // 事件注册
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);

  // shader
  Shader ourShader("../src/27_load_model/shader/vertex.glsl", "../src/27_load_model/shader/fragment.glsl");
  Shader lightObjectShader("../src/27_load_model/shader/light_object_vert.glsl", "../src/27_load_model/shader/light_object_frag.glsl");
  
  // 几何类
  PlaneGeometry planeGeometry(1.0, 1.0, 1.0, 1.0);
  BoxGeometry boxGeometry(1.0, 1.0, 1.0);
  SphereGeometry sphereGeometry(0.1, 10.0, 10.0);

  //纹理
  unsigned int diffuseMap = loadTexture("../static/texture/container2.png");
  unsigned int specularMap = loadTexture("../static/texture/container2_specular.png");
  unsigned int awesomeMap = loadTexture("../static/texture/awesomeface.png");
  ourShader.use();
  ourShader.setInt("material.diffuse", 0);
  ourShader.setInt("material.specular", 1);
  ourShader.setInt("awesomeMap", 2);

  // 模型
  Model ourModel("../static/model/nanosuit/nanosuit.obj");
  //Model ourModel("../static/model/cerberus/Cerberus.obj");


  // 旋转矩阵
  glm::mat4 ex = glm::eulerAngleX(45.0f);
  glm::mat4 ey = glm::eulerAngleY(45.0f);
  glm::mat4 ez = glm::eulerAngleZ(45.0f);
  glm::mat4 qularXYZ = glm::eulerAngleXYZ(45.0f, 45.0f, 45.0f);
  
  // 一些参数
  float fov = 45.0f; // 视锥体的角度
  float radius = 10.0f;  // 摄像机半径
  glm::vec3 view_translate = glm::vec3(0.0, 0.0, -5.0);
  ImVec4 clear_color = ImVec4(25.0 / 255.0, 25.0 / 255.0, 25.0 / 255.0, 1.0); // 25, 25, 25
  glm::vec3 lightPosition = glm::vec3(1.0, 2.5, 2.0); // 光照位置

  // 传递材质属性
  ourShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
  ourShader.setFloat("material.shininess", 32.0f);

  // 设置平行光光照属性
  ourShader.setVec3("directionLight.ambient", 0.01f, 0.01f, 0.01f);
  ourShader.setVec3("directionLight.diffuse", 0.2f, 0.2f, 0.2f); // 将光照调暗了一些以搭配场景
  ourShader.setVec3("directionLight.specular", 1.0f, 1.0f, 1.0f);

  // 设置聚光光照属性
  ourShader.setVec3("spotLight.position", glm::vec3(0.0f, 0.0f, 2.0f));
  ourShader.setVec3("spotLight.direction", camera.Front);
  ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
  ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
  ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
  ourShader.setFloat("spotLight.constant", 1.0f);
  ourShader.setFloat("spotLight.linear", 0.09);
  ourShader.setFloat("spotLight.quadratic", 0.032);
  ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
  ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

  // 设置衰减
  ourShader.setFloat("light.constant", 1.0f);
  ourShader.setFloat("light.linear", 0.09f);
  ourShader.setFloat("light.quadratic", 0.032f);


  // 点光源的位置
  glm::vec3 pointLightPositions[] = {
      glm::vec3(0.7f, 0.2f, 1.5f),
      glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f, 2.0f, -12.0f),
      glm::vec3(0.0f, 0.0f, -3.0f)
  };

  // 点光源颜色
  glm::vec3 pointLightColors[] = {
      glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(1.0f, 0.0f, 1.0f),
      glm::vec3(0.0f, 0.0f, 1.0f),
      glm::vec3(0.0f, 1.0f, 0.0f)
  };

 

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // 时间处理
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastTime;
    lastTime = currentFrame;

    // 在标题中显示帧率信息
    // *************************************************************************
    int fps_value = (int)round(ImGui::GetIO().Framerate);
    int ms_value = (int)round(1000.0f / ImGui::GetIO().Framerate);

    std::string FPS = std::to_string(fps_value);
    std::string ms = std::to_string(ms_value);
    std::string newTitle = "LearnOpenGL - " + ms + " ms/frame " + FPS;
    glfwSetWindowTitle(window, newTitle.c_str());

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //ImGui::Begin("controls");
    //ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //ImGui::End();
    // *************************************************************************

    // 渲染指令
    // ...
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader.use();

    // 修改光源颜色
    glm::vec3 lightColor;
    lightColor.x = sin(glfwGetTime() * 2.0f);
    lightColor.y = sin(glfwGetTime() * 0.7f);
    lightColor.z = sin(glfwGetTime() * 1.3f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, awesomeMap);

    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    glm::vec3 lightPos = glm::vec3(lightPosition.x * glm::sin(glfwGetTime()) * 2.0, lightPosition.y, lightPosition.z);  // 移动光源

    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);

    ourShader.setVec3("directionLight.direction", lightPos); // 光源位置
    ourShader.setVec3("viewPos", camera.Position);           // 观察者位置

    for (unsigned int i = 0; i < 4; i++) {
      // 设置点光源属性
      ourShader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
      ourShader.setVec3("pointLights[" + std::to_string(i) + "].ambient", 0.01f, 0.01f, 0.01f);
      ourShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", pointLightColors[i]);
      ourShader.setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);

      // // 设置衰减
      ourShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
      ourShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
      ourShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
    }

    // 绘制模型
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(15.0f * (float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.13f, 0.13f, 0.13f));
    ourShader.setMat4("model", model);

    ourModel.Draw(ourShader);

    // 绘制灯光物体
    lightObjectShader.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    lightObjectShader.setMat4("model", model);
    lightObjectShader.setMat4("view", view);
    lightObjectShader.setMat4("projection", projection);
    lightObjectShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    glBindVertexArray(sphereGeometry.VAO);
    glDrawElements(GL_TRIANGLES, sphereGeometry.indices.size(), GL_UNSIGNED_INT, 0);

    for (unsigned int i = 0; i < 4; i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, pointLightPositions[i]);

      lightObjectShader.setMat4("model", model);
      lightObjectShader.setVec3("lightColor", pointLightColors[i]);

      glBindVertexArray(sphereGeometry.VAO);
      glDrawElements(GL_TRIANGLES, sphereGeometry.indices.size(), GL_UNSIGNED_INT, 0);
    }

    // 渲染 gui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  boxGeometry.dispose();
  planeGeometry.dispose();
  sphereGeometry.dispose();
  glfwTerminate();

  return 0;
}

// 窗口变动监听
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

// 键盘输入监听
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }

  // 相机按键控制
  // 相机移动
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }
}

// 鼠标移动监听
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

// 加载纹理贴图
unsigned int loadTexture(char const *path)
{
  unsigned int textureID;
  glGenTextures(1, &textureID);

  // 图像y轴翻转
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (data)
  {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}
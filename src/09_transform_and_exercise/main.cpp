#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}


int main(int argc, char *argv[])
{
  // glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 窗口
  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 注册窗口变化监听

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // 设置视口
  glViewport(0, 0, 800, 600);
  // glEnable(GL_PROGRAM_POINT_SIZE);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  
  // 着色器
  Shader ourShader("../src/09_transform_and_exercise/shader/vertex.glsl", "../src/09_transform_and_exercise/shader/fragment.glsl");

  // 顶点数据
  float vertices[] = {
      //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
      0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 右上
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0, 0.0f, // 左下
      -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 左上
  };

  // 索引数据
  unsigned int indices[] = {
      0, 1, 3, // 三角形一
      1, 2, 3  // 三角形二
  };

  // 创建缓冲对象
  unsigned int VBO, EBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // 绑定
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  
  // 填充数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // 设置顶点位置属性指针
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // 设置顶点颜色属性指针
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // 设置顶点纹理坐标属性指针
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  // 生成纹理
  unsigned int texture1, texture2;
  int width, height, nrChannels;

   // 图像y轴翻转
  stbi_set_flip_vertically_on_load(true);

  // 绑定纹理1
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // 设置环绕和过滤方式
  float borderColor[] = {0.3f, 0.1f, 0.7f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

 
  // 加载图片
  unsigned char *data = stbi_load("../static/texture/container.jpg", &width, &height, &nrChannels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);

  // 绑定纹理2
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  // 设置环绕和过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // 加载图片
  data = stbi_load("../static/texture/awesomeface.png", &width, &height, &nrChannels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);


  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);


  // transform
  float factor = 0.0;    //角度变化量

  // 初始化一个单位矩阵
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));                         //缩放   vec3中分别是xyz轴方向上的缩放分量
  trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.0));                    //平移   vec3中分别是xyz轴方向上的平移分量
  trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));   //旋转

  // 将矩阵传递给顶点着色器
  unsigned int transform = glGetUniformLocation(ourShader.ID, "transform");
  glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(trans)); 

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // 渲染指令
    // ...
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader.use();

    factor = glfwGetTime();   //角度随时间变化
    ourShader.setFloat("factor", -factor);    //负数则向逆时针旋转

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO);

    /* 每帧都要变换，所以每次循环  都要重新设置变换矩阵 */
    // 第一个物体
    trans = glm::translate(trans, glm::vec3(-0.5, 0.0, 0.0)); 
    trans = glm::rotate(trans, glm::radians(factor*30.f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(trans));
    trans = glm::mat4(1.0f);    // 传递之后，将trans重置为单位矩阵，防止变换叠加
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_POINTS, 6, GL_UNSIGNED_INT, 0);

    // 第二个物体
    trans = glm::translate(trans, glm::vec3(0.5, 0.0, 0.0));
    trans = glm::scale(trans, glm::vec3(sin(factor * 1.f)*0.5, sin(factor * 1.f)*0.5, 0.5));
    glUniformMatrix4fv(transform, 1, GL_FALSE, glm::value_ptr(trans));
    trans = glm::mat4(1.0f);    // 传递之后，将trans重置为单位矩阵，防止变换叠加
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();
  return 0;
}


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// 窗口大小回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

// 检测ESC输入函数
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    // glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);       // 绑定窗口
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 注册窗口大小回调函数
    
    // 初始化glad
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

     // 设置视口
    glViewport(0, 0, 800, 600);
    

    // 着色器
    Shader ourShader("../src/08_load_texture_exercise/shader/vertex.glsl", 
                        "../src/08_load_texture_exercise/shader/fragment.glsl");


    // 定义顶点数组
    float vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    // 索引数据
    unsigned int indices[] = {
        0, 1, 3, // 三角形1
        1, 2, 3  // 三角形2
    };

    // 创建VBO, VAO, EBO
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    
    // 绑定VBO, VAO, EBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 复制顶点数组/索引数据到缓冲
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
    // 解绑VAO
    glBindVertexArray(0);


    // 纹理
    unsigned int texture1, texture2;
    int width, height, nrChanneles;

    // 图像翻转
    stbi_set_flip_vertically_on_load(true);

    // 绑定纹理1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // 设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);        //S轴/X轴重复纹理图像
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);        //T轴/Y轴重复纹理图像
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   //缩小时采用临近过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    //放大时采用线性过滤
    

    // 加载并生成纹理1  
    unsigned char *data = stbi_load("../static/texture/container.jpg", &width, &height, &nrChanneles, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);    //多级渐远纹理
    }
    else{
        std::cout << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(data);      //释放图像资源

    // 绑定纹理2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // 设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);        //S轴/X轴重复纹理图像
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);        //T轴/Y轴重复纹理图像
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);   //缩小时采用双线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    //放大时采用线性过滤
    
    // 加载并生成纹理2
    data = stbi_load("../static/texture/awesomeface.png", &width, &height, &nrChanneles, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);    //多级渐远纹理
    }
    else{
        std::cout << "Failed to load texture2" << std::endl;
    }
    stbi_image_free(data);      //释放图像资源

    ourShader.use();                  //激活着色器程序
    ourShader.setInt("texture1", 0);  //使用着色器类型
    ourShader.setInt("texture2", 1);  //使用着色器类型

    float factor = 0.0;    //用于按键变化纹理混合度

    // 渲染循环
    while(!glfwWindowShouldClose(window)){
        // 检测ESC
        processInput(window);      

        // 清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // 设置颜色
        glClear(GL_COLOR_BUFFER_BIT);           // 清空上一帧

        // 渲染
        glActiveTexture(GL_TEXTURE0);             // 激活纹理单元1
        glBindTexture(GL_TEXTURE_2D, texture1);    // 绑定纹理1
        glActiveTexture(GL_TEXTURE1);             // 激活纹理单元2
        glBindTexture(GL_TEXTURE_2D, texture2);    // 绑定纹理2
        ourShader.use();                          // 使用着色器

        factor = glfwGetTime();                    // 获取当前时间
        ourShader.setFloat("factor", factor);     // 设置uniform变量

        glBindVertexArray(VAO);               //绑定VAO
        // glDrawArrays(GL_TRIANGLES, 0, 3);                  // 绘制顶点
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 绘制索引
        glBindVertexArray(0);                 //解绑VAO

        
        glfwPollEvents();          // 监听事件
        glfwSwapBuffers(window);   // 交换缓冲区
        
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

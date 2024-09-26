#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

// 顶点着色器
const char *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"   // 位置
"layout (location = 1) in vec3 aColor;\n" // 颜色
"out vec3 ourColor;"            //输出颜色值
"void main(){\n"
    "gl_Position = vec4(aPos, 1.0f);\n"
    "ourColor = aColor;\n"       //把ourColor设置为我们从顶点数据那里得到的输入颜色
"}\n"
;

// 片段着色器
const char *fragmentShaderSource = 
"#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"void main(){\n"
    "FragColor = vec4(ourColor, 1.0f);\n"
"}\n"
; 


int main() {
    // 初始化glfw
    glfwInit();

    // 设置主要和次要版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // 核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);       // 绑定窗口

    // 初始化glad
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 设置视口
    glViewport(0, 0, 800, 600);

    // 注册窗口大小回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // 定义顶点数组
    GLfloat vertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

    // 创建VBO, VAO
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    // 绑定VBO, VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    // 复制顶点数组到缓冲
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 设置顶点位置属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);  
    // 设置顶点颜色属性指针
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); 
    // 解绑VAO
    glBindVertexArray(0);

    // 创建顶点和片段着色器
    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 附加着色器代码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 检测是否编译成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << std::endl;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << std::endl;
    }

    // 着色器附加到程序对象并链接着色器程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << std::endl;
    }

    // 使用着色器程序
    glUseProgram(shaderProgram);

    // 删除着色器  (链接之后就不需要着色器了)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 渲染循环
    while(!glfwWindowShouldClose(window)){
        // 检测ESC
        processInput(window);      

        // 清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // 设置颜色
        glClear(GL_COLOR_BUFFER_BIT);           // 清空上一帧

        // 渲染
        glBindVertexArray(VAO);               //绑定VAO
        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);                 //解绑VAO

        
        glfwPollEvents();          // 监听事件
        glfwSwapBuffers(window);   // 交换缓冲区
        
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

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
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\n"
;

// 片段着色器
const char *fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
        0.5f, 0.5f, 0.0f,    //右上角
        0.5f, -0.5f, 0.0f,   //右下角
        -0.5f, -0.5f, 0.0f,  //左下角                  
        -0.5f, 0.5f, 0.0f    //左上角
    };
    // 索引数组
    unsigned int indexes[] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    // 创建缓冲对象VBO, VAO, EBO
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    // 绑定VBO, VAO, VEO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 复制顶点数组到缓冲(VBO)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 复制索引数组到缓冲(EBO)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // 启用顶点属性
    glEnableVertexAttribArray(0);  
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

    // 设置线框绘制模式
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // 渲染循环
    while(!glfwWindowShouldClose(window)){
        // 检测ESC
        processInput(window);      

        // 清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // 设置颜色
        glClear(GL_COLOR_BUFFER_BIT);           // 清空上一帧

        // 渲染
        glBindVertexArray(VAO);               //绑定VAO, 在绑定VAO的时候会自动绑定EBO
        glUseProgram(shaderProgram);
        // glDrawArrays(GL_TRIANGLES, 0, 3);                  // 绘制顶点
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 绘制索引
        glBindVertexArray(0);                 //解绑VAO

        glfwPollEvents();          // 监听事件
        glfwSwapBuffers(window);   // 交换缓冲区
        
    }

    // 资源释放
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* Todo: 创建两个着色器程序，第二个程序使用一个不同的片段着色器，输出黄色；再次绘制这两个三角形，让其中一个输出为黄色 */

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

// 片段着色器1
const char *fragmentShaderSource1 = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n"
; 

// 片段着色器2
const char *fragmentShaderSource2 = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
    "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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
        //第一个三角形
        -0.5f, 0.5f, 0.0f,
        -0.75f, -0.5f, 0.0f,
        -0.25f, -0.5f, 0.0f,
        //第二个三角形
        0.5f, 0.5f, 0.0f,
        0.75f, -0.5f, 0.0f,
        0.25f, -0.5f, 0.0f 
    };
    

    // 创建缓冲对象VBO, VAO
    unsigned int VBOs[2], VAOs[2];
    glGenBuffers(2, VBOs);
    glGenVertexArrays(2, VAOs);

    // 1.绑定VBO1, VAO1
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBindVertexArray(VAOs[0]);
    // 复制顶点数组到缓冲(VBO)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 2.绑定VBO2, VAO2
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBindVertexArray(VAOs[1]);
    // 复制顶点数组到缓冲(VBO)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(9*sizeof(float)));
    glEnableVertexAttribArray(0);

    // 3.解绑VAO
    glBindVertexArray(0);

    // 创建顶点和片段着色器
    unsigned int vertexShader, fragmentShader1, fragmentShader2;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    // 附加着色器代码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader1);
    glCompileShader(fragmentShader2);
    // 检测是否编译成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << std::endl;
    }
    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT1::COMPILATION_FAILED\n" << std::endl;
    }
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n" << std::endl;
    }
    

    // 着色器附加到程序对象并链接着色器程序
    unsigned int shaderProgram1, shaderProgram2;
    shaderProgram1 = glCreateProgram();
    shaderProgram2 = glCreateProgram();

    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glAttachShader(shaderProgram2, fragmentShader2);

    glLinkProgram(shaderProgram1);
    glLinkProgram(shaderProgram2);
    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM1::LINKING_FAILED\n" << std::endl;
    }
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM2::LINKING_FAILED\n" << std::endl;
    }


    // 删除着色器  (链接之后就不需要着色器了)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);

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
        glBindVertexArray(VAOs[0]);           //绑定VAO, 在绑定VAO的时候会自动绑定EBO
        glUseProgram(shaderProgram1);
        glDrawArrays(GL_TRIANGLES, 0, 3);     // 绘制顶点

        glBindVertexArray(VAOs[1]);           //绑定VAO, 在绑定VAO的时候会自动绑定EBO
        glUseProgram(shaderProgram2);
        glDrawArrays(GL_TRIANGLES, 0, 3);     // 绘制顶点
        
        glBindVertexArray(0);                 //解绑VAO

        glfwPollEvents();          // 监听事件
        glfwSwapBuffers(window);   // 交换缓冲区
        
    }

    // 资源释放
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);

    glfwTerminate();
    return 0;
}

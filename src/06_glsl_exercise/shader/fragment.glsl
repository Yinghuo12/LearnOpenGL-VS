#version 330 core

in vec3 ourColor;


/** Todo3: 使用out关键字把顶点位置输出到片段着色器，
    并将片段的颜色设置为与顶点位置相等（来看看连顶点位置值都在三角形中被插值的结果）
    做完这些后，尝试回答下面的问题：为什么在三角形的左下角是黑的?
**/ 
in vec3 ourPos;  


out vec4 FragColor; 

void main(){
    FragColor = vec4(ourPos, 1.0f);
    // FragColor = vec4(ourColor, 1.0f);
}
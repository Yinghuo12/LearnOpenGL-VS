#version 330 core
layout (location = 0) in vec3 aPos;   // 位置
layout (location = 1) in vec3 aColor; // 颜色

out vec3 ourColor;            //输出颜色值
void main(){
    gl_Position = vec4(aPos, 1.0f);
    ourColor = aColor;       //把ourColor设置为我们从顶点数据那里得到的输入颜色
}
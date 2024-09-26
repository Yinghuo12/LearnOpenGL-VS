#version 330 core
layout (location = 0) in vec3 aPos;   // 位置
layout (location = 1) in vec3 aColor; // 颜色

out vec3 ourColor;            //输出颜色值
out vec3 ourPos;              //输出位置值
uniform float xOffset;        //水平偏移量

void main(){
    /** Todo1: 修改顶点着色器让三角形上下颠倒 **/
    // gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0f);

     /** Todo2:使用uniform定义一个水平偏移量，在顶点着色器中使用这个偏移量把三角形移动到屏幕右侧 **/
    // gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0f);

    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);

    
    ourColor = aColor;
    ourPos = aPos;
}
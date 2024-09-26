#version 330 core

in vec3 ourColor;
in vec2 ourTexCoord;
out vec4 FragColor;

// 采样器
uniform sampler2D texture1; 
uniform sampler2D texture2;


void main(){
    // FragColor = texture2D(texture1, ourTexCoord) * vec4(ourColor, 1.0f);   //混合顶点颜色与纹理1颜色
    // FragColor = texture2D(texture2, ourTexCoord) * vec4(ourColor, 1.0f);   //混合顶点颜色与纹理2颜色
    FragColor = mix(texture2D(texture1, ourTexCoord), texture2D(texture2, ourTexCoord), 0.2);   //混合纹理1和纹理2颜色
    // FragColor = vec4(ourColor, 1.0f);
}
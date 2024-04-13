#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;  
layout(location = 1) in vec2 aTex;  
layout(location = 2) in vec2 aTexID;  
layout(location = 3) in vec2 aCol;  

uniform mat4 u_ProjMatrix;

void main()
{
    gl_Position = u_ProjMatrix * vec4(aPos, 1.0f);
}


#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

void main()
{
    color = (1.f, 1.f, 1.f, 1.f);
}
#shader vertex
#version 330 core
layout(location = 0) in vec3 iPos;  
layout(location = 1) in vec2 iUV;  
layout(location = 2) in vec4 iCol; 
layout(location = 3) in float iTexID;

uniform mat4 u_projMatrix;

out vec2 vUV;
out vec4 vCol;
out float vTexID;

void main()
{
	gl_Position = u_projMatrix * vec4(iPos, 1.f);
	vUV = iUV;
	vCol = vec4(iCol.x/255.f, iCol.y/255.f, iCol.z/255.f, iCol.w/255.f);
	vTexID = iTexID;
}

#shader fragment
#version 330 core
layout(location = 0) out vec4 outCol;

in vec2 vUV;
in vec4 vCol;
in float vTexID;
const int fragTexSlotAmm = 32; //make it platform dependent
uniform sampler2D u_Tex[fragTexSlotAmm];

void main()
{
    outCol = texture(u_Tex[int(vTexID)], vUV) * vCol;
}
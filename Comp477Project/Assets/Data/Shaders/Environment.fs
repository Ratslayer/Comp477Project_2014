#version 150

uniform samplerCube environment;

in vec3 vTexCoord;
out vec4 fragmentColor;

void main()
{   
    fragmentColor = clamp( texture( environment, vTexCoord ), 0.0, 1.0 );
}


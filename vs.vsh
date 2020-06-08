attribute highp vec4 qt_Vertex;
attribute highp vec4 qt_MultiTexCoord0;
uniform highp mat4 qt_ModelViewProjectionMatrix;
varying highp vec4 qt_TexCoord0;

#version 430 core
layout(location = 0) in vec4 vPosition;
void main()
{
gl_Position = vPosition;
}

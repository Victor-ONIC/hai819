#version 460

layout (location = 0) in vec4 vsiPosition;

uniform mat4 MVP;

out  vec4 vsoColor;

void main(void) {
  //vsoModPos = viewMatrix * modelMatrix * vec4(vsiPosition, 1.0);
  //gl_Position = projectionMatrix * vsoModPos;
  vsoColor = vec4(vsiPosition.xyz, 1.0);
  gl_Position = MVP * vec4(vsiPosition.xyz, 1.0);
}

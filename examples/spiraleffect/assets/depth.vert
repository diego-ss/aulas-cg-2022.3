#version 300 es

layout(location = 0) in vec3 inPosition;

uniform vec4 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec4 fragColor;

void main() {
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);

  // o tom da cor varia de acordo com a distância e a altura 
  float i = 1.0 - (-posEyeSpace.z / 50.0) + (posEyeSpace.y / 50.0);
  fragColor = vec4(i, i, i, 1) * color;

  gl_Position = projMatrix * posEyeSpace;
}
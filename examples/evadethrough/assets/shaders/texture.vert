#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

uniform mediump vec4 lightPos;

uniform mediump int mappingMode;


out vec3 fragV;
out vec3 fragL;
out vec3 fragN;
out vec2 fragTexCoord;
out vec3 fragPObj;
out vec3 fragNObj;
out vec4 wL;
out vec3 wP;

void main() {
  vec3 P = (viewMatrix * modelMatrix * vec4(inPosition, 1.0)).xyz;
  vec3 N;
  if (mappingMode == 2)
    N = normalMatrix * vec3(0.0, -1.0, 0.0);
  else
    N = normalMatrix * inNormal;
  vec4 lP = viewMatrix * lightPos;
  vec3 L = -(lP - vec4(P, 1.0)).xyz;

  wL = lightPos;
  wP = (modelMatrix * vec4(inPosition, 1.0)).xyz;

  fragL = L;
  fragV = -P;
  fragN = N;
  fragTexCoord = inTexCoord;
  fragPObj = inPosition;
  fragNObj = inNormal;

  gl_Position = projMatrix * vec4(P, 1.0);
}

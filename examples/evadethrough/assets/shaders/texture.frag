#version 410

in vec3 fragN;
in vec3 fragL;
in vec3 fragV;
in vec2 fragTexCoord;
in vec3 fragPObj;
in vec3 fragNObj;

// Light properties
uniform vec4 Ia, Id, Is;

// Material properties
uniform vec4 Ka, Kd, Ks;
uniform float shininess;

// Diffuse texture sampler
uniform sampler2D diffuseTex;

uniform int mappingMode;

out vec4 outColor;

// Blinn-Phong reflection model
vec4 BlinnPhong(vec3 N, vec3 L, vec3 V, vec2 texCoord) {
  vec4 map_Kd;
  vec4 map_Ka;

  N = normalize(N);
  L = normalize(L);

  // Compute lambertian term
  float lambertian = max(dot(N, L), 0.0);

  // Compute specular term
  float specular = 0.0;
  if (lambertian > 0.0) {
    V = normalize(V);
    vec3 H = normalize(L + V);
    float angle = max(dot(H, N), 0.0);
    specular = pow(angle, shininess);
  }

  if (mappingMode == 0) {
    map_Kd = texture(diffuseTex, texCoord);
    map_Ka = map_Kd;
  } else {
    map_Kd = vec4(1, 1, 1, 1);
    map_Ka = map_Kd;
  }

  // map_Kd = texture(diffuseTex, texCoord);
  // map_Ka = map_Kd;


  // vec4 diffuseColor =  Kd * Id * lambertian;
  // vec4 specularColor = Ks * Is * specular;
  // vec4 ambientColor =  Ka * Ia;

  vec4 diffuseColor = map_Kd * Kd * Id * lambertian;
  vec4 specularColor = Ks * Is * specular;
  vec4 ambientColor = map_Ka * Ka * Ia;

  return ambientColor + diffuseColor + specularColor;
}


void main() {
  vec4 color;
  vec2 texCoord;

  // if (mappingMode == 0) {
  //   texCoord = fragTexCoord;
  // } else {
  //   texCoord = vec2(-1, -1);
  // }

  texCoord = fragTexCoord;

  color = BlinnPhong(fragN, fragL, fragV, texCoord);


  outColor = color;

  // if (gl_FrontFacing) {
  //   outColor = color;
  // } else {
  //   float i = (color.r + color.g + color.b) / 3.0;
  //   outColor = vec4(i, 0, 0, 1.0);
  // }
}

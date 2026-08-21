#version 430 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPosition;
in vec2 atlasSizes;
in vec2 Repeat;

uniform int lightCount;
uniform int u_HasNormalMap;

struct Light {
  vec3 position;
  vec3 color;
  float strength;
};

layout(std430, binding = 0) buffer LightsBuffer {
  Light lights[];
};

uniform sampler2D texture0; // Albedo
uniform sampler2D texture1; // Normals


vec2 mat = vec2(0.0, 0.0);

vec2 hash22(vec2 p) {
  vec3 p3 = fract(vec3(p.xyx) * vec3(443.897, 441.423, 437.195));
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.xx + p3.yz) * p3.zy);
}

void main() {
  vec2 globalUV = (TexCoords * atlasSizes * Repeat) / (atlasSizes * 2.0f);

  vec2 cellID = floor(globalUV);
  vec2 localUV = fract(globalUV);

  vec2 randVal = hash22(cellID);
  vec2 mat = floor(randVal * atlasSizes);

  vec2 tex_cord = (mat + localUV) / atlasSizes;

  vec4 albedo = texture(texture0, tex_cord);

  if (albedo.a <= 0.05) discard;

  vec3 result = vec3(0.0);

  if (u_HasNormalMap == 1) {
    vec3 normal = texture(texture1, tex_cord).rgb * 2.0 - 1.0;
      normal.x *= -1.0;
      normal = normalize(normal);

      for (int i = 0; i < lightCount; ++i) {
        vec3 lightPos = lights[i].position;

        vec3 toLight = lightPos - FragPosition;
        
        toLight.x *= -1.0; 

        float distance = length(toLight);
        if (distance == 0.0) distance = 0.0001;

        vec3 lightDir = toLight / distance;

        float diff = max(dot(normal, lightDir), 0.0);

        result += albedo.rgb * 
                  lights[i].color * 
                  lights[i].strength * 
                  diff;
    }
  } else {
    result = albedo.rgb;
  }

  FragColor = vec4(result, albedo.a);
}







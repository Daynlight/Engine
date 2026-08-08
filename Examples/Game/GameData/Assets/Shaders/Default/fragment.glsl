#version 430 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPosition;

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

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
  vec4 albedo = texture(texture0, TexCoords);

    if (albedo.a <= 0.05) discard;

    vec3 result = vec3(0.0);

    if (u_HasNormalMap == 1) {
      vec3 normal = texture(texture1, TexCoords).rgb * 2.0 - 1.0;
      normal.x *= -1.0;
      normal.y *= -1.0;
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


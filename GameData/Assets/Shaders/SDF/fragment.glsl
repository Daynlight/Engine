#version 430 core

in vec2 TexCoords;
out vec4 FragColor;

uniform mat4 transformation;
uniform int lightCount;
uniform vec3 cameraPosition;
uniform int material_id;

struct Light {
  vec3 position;
  vec3 color;
  float strength;
};

layout(std430, binding = 0) buffer LightsBuffer {
  Light lights[];
};

struct Material {
  vec3 albedo;
  float metallic;
  float roughness;
  vec3 emission_color;
  float emission_strength;
  float ambient_occlusion;
};

layout(std430, binding = 1) buffer MaterialsBuffer {
  Material material[];
};

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness){
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;
  return a2 / max(denom, 0.0001);
}

float GeometrySchlickGGX(float NdotV, float roughness){
  float r = roughness + 1.0;
  float k = (r * r) / 8.0;
  return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

vec3 FresnelSchlick(vec3 V, vec3 H, vec3 albedo, float metallic){
  float cosTheta = max(dot(V, H), 0.0);  
  vec3 F0 = vec3(0.04);
  F0 = mix(F0, albedo, metallic);
  return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float Calc_D(float NdotH, float roughness){
  float a = roughness * roughness;
  float a2 = a * a;
  float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
  float D = a2 / (PI * denom * denom + 0.0001);
  return D;
}

float GGX(float NdotV, float NdotL, float roughness){
  float k = (roughness + 1.0);
  k = (k * k) / 8.0;
  float G1 = NdotL / (NdotL * (1.0 - k) + k);
  float G2 = NdotV / (NdotV * (1.0 - k) + k);
  float G = G1 * G2;
  return G;
}

vec3 Specular(float NdotL, float NdotV, float D, float G, vec3 Fresnel){
  return (D * G * Fresnel) / max(4.0 * NdotL * NdotV, 0.001);
}

vec3 Diffuse(vec3 Fresnel, vec3 albedo, float metallic){
  vec3 kS = Fresnel;
  vec3 kD = (1.0 - kS) * (1.0 - metallic);
  return kD * albedo / PI;
}

vec3 BRDF(vec3 Normal, vec3 FragPos, vec3 cameraPos, vec3 lightPos, vec3 lightColor, vec3 albedo, float metallic, float roughness, vec3 emission_color, float emission_strength, float ambient_occlusion) {
  vec3 N = normalize(Normal);
  vec3 V = normalize(cameraPos - FragPos);
  vec3 L = normalize(lightPos - FragPos);
  vec3 H = normalize(V + L);
  float NdotL = max(dot(N, L), 0.0);
  float NdotV = max(dot(N, V), 0.0);
  float NdotH = max(dot(N, H), 0.0);
  vec3 Fresnel = FresnelSchlick(V, H, albedo, metallic);
  float D = Calc_D(NdotH, roughness);
  float G = GGX(NdotV, NdotL, roughness);
  vec3 specular = Specular(NdotL, NdotV, D, G, Fresnel);
  vec3 diffuse = Diffuse(Fresnel, albedo, metallic); 
  vec3 lighting = (diffuse + specular) * lightColor * NdotL;
  vec3 ambient = 0.03 * albedo * ambient_occlusion;
  vec3 emissiveColor = emission_color * emission_strength;
  return lighting + ambient + emissiveColor;
}

float sdSphere(vec3 p, vec3 center, float radius) {
  return length(p - center) - radius;
}

float sdBox(vec3 p, vec3 center, vec3 b) {
  vec3 q = abs(p - center) - b;
  return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sdTorus(vec3 p, vec3 center, vec2 t) {
  vec3 translatedP = p - center;
  vec2 q = vec2(length(translatedP.xz) - t.x, translatedP.y);
  return length(q) - t.y;
}





float map(vec3 p) {
  float minDist = 100000.0; 
  float sphere1 = sdSphere(p, vec3(0.0, 200.0, 0.0), 15.0);
  minDist = min(minDist, sphere1);
  float box1 = sdBox(p, vec3(40.0, 200.0, 0.0), vec3(10.0, 10.0, 10.0));
  minDist = min(minDist, box1);
  float torus1 = sdTorus(p, vec3(80.0, 200.0, 0.0), vec2(12.0, 4.0));
  minDist = min(minDist, torus1);
  return minDist;
}

vec3 calculateNormal(vec3 p) {
  vec2 eps = vec2(0.005, 0.0);
  return normalize(vec3(
    map(p + eps.xyy) - map(p - eps.xyy),
    map(p + eps.yxy) - map(p - eps.yxy),
    map(p + eps.yyx) - map(p - eps.yyx)
  ));
}

void main() {
  vec2 uv = TexCoords * 2.0 - 1.0;
  mat4 invTransformation = inverse(transformation);
  vec4 nearTarget = invTransformation * vec4(uv, -1.0, 1.0);
  vec4 farTarget  = invTransformation * vec4(uv,  1.0, 1.0);
  vec3 ro = nearTarget.xyz / nearTarget.w;
  vec3 rd = normalize((farTarget.xyz / farTarget.w) - ro);
  float t = 0.0;
  float max_t = 4000.0; 
  bool hit = false;
  vec3 hitPos = vec3(0.0);
  for(int i = 0; i < 200; i++) { 
    vec3 p = ro + rd * t;
    float d = map(p);
    if(d < 0.005) { 
      hit = true;
      hitPos = p;
      break;
    }
    if(t > max_t) break;
    t += d;
  }
  if(!hit) {
    discard; 
  }
  vec4 clipPos = transformation * vec4(hitPos, 1.0);
  float ndcDepth = clipPos.z / clipPos.w;
  gl_FragDepth = ndcDepth * 0.5 + 0.5;
  vec3 normal = calculateNormal(hitPos);
  vec3 totalLighting = vec3(0.0);
  for(int i = 0; i < lightCount; i++) {
    totalLighting += BRDF(
      normal,
      hitPos,
      cameraPosition,
      lights[i].position,
      lights[i].color,
      material[material_id].albedo,
      material[material_id].metallic,
      material[material_id].roughness,
      material[material_id].emission_color,
      material[material_id].emission_strength,
      material[material_id].ambient_occlusion
    ) * lights[i].strength;
  }
  FragColor = vec4(totalLighting, 1.0);
}

#version 430 core

in vec2 TexCoords;
out vec4 FragColor;

// Expects your camera's forward matrix: (Projection * View)
uniform mat4 transformation;

struct SDFObject {
    ivec4 type_pad;      // x = type (0: Sphere, 1: Box, etc.)
    vec4  position_pad;  // xyz = world-space position
    vec4  size_pad;      // xyz = dimensions / size parameters
};

layout(std430, binding = 3) buffer SDFBuffer {
    SDFObject objects[];
};

// ==========================================
// ====== MODULAR SHAPE FUNCTIONS (SDFs) ====
// ==========================================

// Type 0: Sphere
float sdSphere(vec3 p, vec3 center, float radius) {
    return length(p - center) - radius;
}

// Type 1: Box (Cube)
float sdBox(vec3 p, vec3 center, vec3 b) {
    vec3 q = abs(p - center) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

// ==========================================
// ====== SCENE MAPPING & LOOPING ===========
// ==========================================

float map(vec3 p) {
    float minDist = 100000.0; 
    int numObjects = objects.length(); 
    
    for(int i = 0; i < numObjects; i++) {
        int type   = objects[i].type_pad.x;
        vec3 pos   = objects[i].position_pad.xyz;
        vec3 size  = objects[i].size_pad.xyz;
        
        float d = 100000.0;
        
        // Modular shape selection
        if (type == 0) {
            d = sdSphere(p, pos, size.x); // size.x acts as radius
        } 
        else if (type == 1) {
            d = sdBox(p, pos, size);     // size.xyz acts as half-extents
        }
        // Add more primitives here (e.g., type == 2 for Cylinder)
        
        minDist = min(minDist, d);
    }
    return minDist;
}

// Calculates clean, world-space normals for lighting
vec3 calculateNormal(vec3 p) {
    vec2 eps = vec2(0.005, 0.0);
    return normalize(vec3(
        map(p + eps.xyy) - map(p - eps.xyy),
        map(p + eps.yxy) - map(p - eps.yxy),
        map(p + eps.yyx) - map(p - eps.yyx)
    ));
}

// ==========================================
// ====== MAIN RAYMARCHING PIPELINE =========
// ==========================================

void main() {
    // 1. Convert incoming UV coordinates to Normalized Device Coordinates [-1, 1]
    vec2 uv = TexCoords * 2.0 - 1.0;
    
    // 2. CRITICAL FIX: Explicitly compute the inverse matrix inside GLSL
    mat4 invTransformation = inverse(transformation);
    
    // 3. Unproject NDC back to true world-space positions
    vec4 nearTarget = invTransformation * vec4(uv, -1.0, 1.0);
    vec4 farTarget  = invTransformation * vec4(uv,  1.0, 1.0);
    
    // 4. Perspective divide to generate flawless mathematical world rays
    vec3 ro = nearTarget.xyz / nearTarget.w;
    vec3 rd = normalize((farTarget.xyz / farTarget.w) - ro);
    
    // 5. Raymarch the scene data
    float t = 0.0;
    float max_t = 4000.0; // Pushed far out to match your game camera configurations
    bool hit = false;
    vec3 hitPos = vec3(0.0);
    
    for(int i = 0; i < 200; i++) { 
        vec3 p = ro + rd * t;
        float d = map(p);
        
        if(d < 0.005) { // Sharp hit surface threshold
            hit = true;
            hitPos = p;
            break;
        }
        if(t > max_t) break;
        t += d;
    }
    
    // 6. Blending with the original FBO pipeline
    if(!hit) {
        discard; 
    }
    
    // 7. Shading and Output
    vec3 normal = calculateNormal(hitPos);
    vec3 lightDir = normalize(vec3(0.4, 1.0, -0.3)); // Directional scene light match
    
    float diff = max(dot(normal, lightDir), 0.25);   // Ambient wrap floor included
    
    // Renders the buffer objects with clean diffuse shading
    FragColor = vec4(vec3(0.9, 0.9, 0.9) * diff, 1.0);
}

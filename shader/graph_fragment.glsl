#version 450

layout(location = 0) in vec3 object_color;
layout(location = 1) in vec3 world_normal;

// out
layout(location = 0) out vec4 final_color;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0f, -3.0f, -1.0f));
const float AMBIENT = 0.02f;

void main() {
    float intensity = AMBIENT + max(dot(world_normal, DIRECTION_TO_LIGHT), 0.2f);
    final_color = vec4(intensity * object_color, 1.0f);
}

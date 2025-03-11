#version 460

layout(std430, binding = 0) buffer BlockData {
    uint data[]; // Le tableau de données
};

uniform int map_width;
uniform int map_height;
uniform int map_depth;
uniform vec4 Lp;
uniform mat4 MVP;
uniform mat4 view;
uniform sampler2D grass_tex;
uniform sampler2D water_tex;

out uint id;
out vec3 normal;

// Fonction pour convertir (x, y, z) en un index 1D
int index3DTo1D(int x, int y, int z, int width, int height) {
    return x + y * width + z * (width * height);
}

// Fonction pour convertir un index 1D en coordonnées 3D (x, y, z)
vec3 index1DTo3D(uint index, uint width, uint depth) {
    uint y = index / (width * depth);
    uint z = (index % (width * depth)) / width;
    uint x = index % width;
    return vec3(x, y, z);
}

void main(void) {
  int index1D = gl_VertexID;  // Indice automatique
  vec3 xyz = index1DTo3D(index1D, map_width, map_depth);
  id = data[index1D];
  gl_Position = vec4(xyz, 1.0);
}

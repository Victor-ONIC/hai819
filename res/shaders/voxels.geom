#version 450

layout(points) in;  // Prend un seul point en entrée
layout (triangle_strip, max_vertices = 24) out;

uniform int map_width;
uniform int map_height;
uniform int map_depth;
uniform mat4 MVP;

in uint id[];
out uint frag_id[1];

void main() {
    vec3 pos = gl_in[0].gl_Position.xyz; // Position du point reçu

    // Définition des 8 sommets du cube
    float size = 0.5; // Taille du cube
    vec3 offsets[8] = vec3[](
        vec3(-size, -size, -size), vec3(size, -size, -size),
        vec3(size,  size, -size), vec3(-size,  size, -size),
        vec3(-size, -size,  size), vec3(size, -size,  size),
        vec3(size,  size,  size), vec3(-size,  size,  size)
    );

    // Indices des sommets pour former 6 faces en triangle strip
    int indices[24] = int[](
        0, 1, 3, 2,  // Face arrière
        2, 1, 6, 5,  // Face droite
        5, 4, 6, 7,  // Face avant
        7, 3, 4, 0,  // Face gauche
        7, 6, 3, 2,  // Face haut
        0, 1, 4, 5   // Face bas
    );

    frag_id[0] = id[0];

    // Générer les faces du cube
    for (int i = 0; i < 24; i++) {
        gl_Position = MVP * vec4(pos + offsets[indices[i]], 1.0);
        EmitVertex();

        if (i % 4 == 3) {
            EndPrimitive(); // Terminer la face actuelle
        }
    }
}

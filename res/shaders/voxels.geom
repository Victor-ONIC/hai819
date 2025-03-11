#version 460

layout(points) in;  // Prend un seul point en entrée
layout (triangle_strip, max_vertices = 24) out;

uniform int map_width;
uniform int map_height;
uniform int map_depth;
uniform vec4 Lp;
uniform mat4 MVP;
uniform mat4 view;
uniform sampler2D grass_tex;
uniform sampler2D water_tex;

in uint id[];
out uint frag_id[1];
out vec2 coord_tex;
out vec3 normal;
out vec4 gsoModPos;
out vec3 gsoNormal;

void main() {
    frag_id[0] = id[0]; // Transmission de l'id au fs
    vec3 pos = gl_in[0].gl_Position.xyz; // Position du point reçu
    vec3 v0, v1, v2;

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

    vec2 uv_map[24] = vec2[](
        vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(0.0, 0.0), vec2(1.0, 0.0),  // Face arrière
        vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),  // Face droite
        vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(0.0, 0.0), vec2(1.0, 0.0),  // Face avant
        vec2(1.0, 1.0), vec2(0.0, 1.0), vec2(1.0, 0.0), vec2(0.0, 0.0),  // Face gauche
        vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),  // Face haut
        vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(0.0, 0.0), vec2(1.0, 0.0)   // Face bas
);

    vec3 cube_Normal[6] = vec3[](
        vec3( 0.0,  0.0, -1.0),  // Face arrière
        vec3( 1.0,  0.0,  0.0),  // Face droite
        vec3( 0.0,  0.0,  1.0),  // Face avant
        vec3(-1.0,  0.0,  0.0),  // Face gauche
        vec3( 0.0,  1.0,  0.0),  // Face haut
        vec3( 0.0, -1.0,  0.0)   // Face bas
);

    // Calcul des normales
    mat4 N = transpose(inverse(view));
    vec4 Lpv = view * Lp;
    for (int i = 0; i < 6; i++){
        cube_Normal[i] = normalize((N * vec4(cube_Normal[i], 0.0)).xyz);
    }

    // Générer les faces du cube
    for (int i = 0; i < 24; i++) {
        vec4 worldpos = vec4(pos + offsets[indices[i]], 1.0);
        gsoModPos = view * vec4(gl_Position.xyz, 1.0);
        vec3 Ld = normalize(gsoModPos - Lpv).xyz;
        coord_tex = uv_map[i];
        gsoNormal = cube_Normal[i/4];
        gl_Position = MVP * worldpos;
        EmitVertex();

        if (i % 4 == 3) {
            EndPrimitive(); // Terminer la face actuelle
        }
    }
}

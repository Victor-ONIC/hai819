#version 460 core

layout (location = 0) in uint blockType;
layout (location = 1) in vec4 vert0;
layout (location = 2) in vec4 vert1;
layout (location = 3) in vec4 vert2;
layout (location = 4) in vec4 vert3;
layout (location = 5) in vec4 normal;
layout (location = 6) in vec2 coord_tex0;
layout (location = 7) in vec2 coord_tex1;
layout (location = 8) in vec2 coord_tex2;
layout (location = 9) in vec2 coord_tex3;

uniform vec4 Lp;
uniform mat4 MVP;
uniform mat4 view;
uniform sampler2D grass_tex;
uniform sampler2D water_tex;
uniform sampler2D cobble_tex;

flat out uint fragBlockType;
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec4 gsoModPos;

void main()
{
    // Tableau local des sommets du quad
    vec4 verts[4] = vec4[4](vert0, vert1, vert2, vert3);
    vec2 coords_tex[4] = vec2[4](coord_tex0, coord_tex1, coord_tex2, coord_tex3);

    // Indices pour construire deux triangles : 0,1,2 et 2,3,0
    int indices[6] = int[6](0, 1, 2, 2, 1, 3);

    // Calcul du sommet courant et de la texcoord associée
    vec4 pos = verts[indices[gl_VertexID]];
    //pos = vec4(pos.x, pos.y, pos.z, pos.w);
    vec2 texCoord = coords_tex[indices[gl_VertexID]]; // Renommé pour éviter la confusion

    // Projection et view
    gl_Position = MVP * pos;
    gsoModPos = view * vec4(gl_Position.xyz, 1.0);

    // Sorties
    fragNormal = normalize(normal.xyz);
    fragTexCoord = texCoord; // Envoi des coordonnées de texture à fragment shader
    fragBlockType = blockType;
}

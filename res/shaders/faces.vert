#version 460 core

layout (location = 0) in uint blockType;
layout (location = 1) in vec4 vert0;
layout (location = 2) in vec4 vert1;
layout (location = 3) in vec4 vert2;
layout (location = 4) in vec4 vert3;
layout (location = 5) in vec4 normal;
layout (location = 6) in vec2 coord_tex;


uniform vec4 Lp;
uniform mat4 MVP;
uniform mat4 view;
uniform sampler2D grass_tex;
uniform sampler2D water_tex;

out vec3 fragNormal;
// TODO Envoyer 4 texcoord
out vec2 fragTexCoord;
flat out uint fragBlockType;

void main()
{
    // Tableau local des sommets du quad
    vec4 verts[4] = vec4[4](vert0, vert1, vert2, vert3);

    // Indices pour construire deux triangles : 0,1,2 et 2,3,0
    int indices[6] = int[6](0, 1, 2, 2, 3, 0);

    vec4 pos = verts[indices[gl_VertexID]];
    gl_Position = MVP * pos;

    fragNormal = normalize(normal.xyz);
    fragTexCoord = coord_tex;
    fragBlockType = blockType;
}

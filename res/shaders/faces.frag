#version 460 core

in flat uint fragBlockType;
in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 fragColor;

uniform sampler2D grass_tex;
uniform sampler2D water_tex;
uniform vec4 Lp;  // Position de la lumière
uniform mat4 view;

void main(void) {
    vec3 Lpv = view * Lp;
    vec3 Ld = normalize(Lpv.xyz - fragTexCoord);  // Direction de la lumière
    float phongIL = max(dot(fragNormal, -Ld), 0.0);  // Phong lighting

    // Différentes textures basées sur le type de bloc
    if (fragBlockType == 1) {
        vec4 texColor = texture(grass_tex, fragTexCoord);
        fragColor = vec4(phongIL * texColor.rgb, 1.0);
    } else if (fragBlockType == 2) {
        vec4 texColor = texture(water_tex, fragTexCoord);
        fragColor = vec4(0.4 * texColor.rgb, 0.12);
    }
    // Ajoutez d'autres types de blocs selon les besoins
}

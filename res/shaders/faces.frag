#version 460 core

in flat uint fragBlockType;
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec4 gsoModPos;

out vec4 fragColor;

uniform sampler2D grass_tex;
uniform sampler2D water_tex;
uniform sampler2D cobble_tex;
uniform vec4 Lp;  // Position de la lumière
uniform mat4 view;

void main(void) {
  vec4 Lpv = Lp;
  //vec4 Lpv = view * Lp;
  vec3 Ld = normalize(gsoModPos - Lpv).xyz;
  float phongIL = 0.0 + clamp(dot(-Ld, fragNormal), 0.0, 1.0);
  //float phongIL = 1.0;

    if(fragBlockType == 1){
        //vec4 texColor = texture(cobble_tex, fragTexCoord);
        vec4 texColor = vec4(0.2, 0.2, 0.2, 1.0);
        fragColor = vec4((phongIL * texColor.rgb), 1.0);
    }
    else if(fragBlockType == 2){
        vec4 texColor = vec4(0.1, 0.15, 0.55, 1.0);
        //vec4 texColor = texture(water_tex, fragTexCoord);
        //fragColor = vec4((phongIL * texColor.rgb), 1.0);
        fragColor = vec4((0.6 * texColor.rgb), 0.70);
    }
    else if(fragBlockType == 3){
        vec4 texColor = vec4(0.15, 0.35, 0.1, 1.0);
        //vec4 texColor = texture(grass_tex, fragTexCoord);
        fragColor = vec4((phongIL * texColor.rgb), 1.0);
    }
}

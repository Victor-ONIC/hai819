#version 460

in flat uint frag_id[1];
in vec2 coord_tex;
in vec4 gsoModPos;
in vec3 gsoNormal;

out vec4 fragColor;

uniform int map_width;
uniform int map_height;
uniform int map_depth;
uniform vec4 Lp;
uniform mat4 MVP;
uniform mat4 view;
uniform sampler2D grass_tex;
uniform sampler2D water_tex;

void main(void) {

  vec4 Lpv = view * Lp;
  vec3 Ld = normalize(gsoModPos - Lpv).xyz;
  float phongIL = clamp(dot(-Ld, gsoNormal), 0.0, 1.0);

    if(frag_id[0] == 0){  discard;}  // Vide
    if(frag_id[0] == 1){
        vec4 texColor = texture(grass_tex, coord_tex);
        fragColor = vec4((phongIL * texColor.rgb), 1.0);
        //fragColor = vec4(normalize(gsoNormal) * 0.5 + 0.5, 1.0); //  Afficher les normals aux faces
        //fragColor = phongIL * vec4(0.0, 1.0, 0.0, 1.0);
    }
    else if(frag_id[0] == 2){
        vec4 texColor = texture(water_tex, coord_tex);
        fragColor = vec4((phongIL * texColor.rgb), 0.07);
        //fragColor = vec4(normalize(gsoNormal) * 0.5 + 0.5, 1.0); //  Afficher les normals aux faces
        //fragColor = phongIL * vec4(0.0, 0.0, 1.0, 1.0);
    }
}

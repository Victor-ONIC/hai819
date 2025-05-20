#version 460

in flat uint fragBlockType;
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec4 gsoModPos;
in vec4 vsoPos;

out vec4 fragColor;

uniform sampler2D grass_tex;
uniform sampler2D water_tex;
uniform sampler2D cobble_tex;
uniform vec4 Lp;  // Position de la lumière
uniform mat4 MVP;
uniform mat4 view;
uniform mat4 proj;
uniform ivec2 offset;
uniform vec3 cam;

void main(void) {
    //fog
    vec4 viewSpacePos = view * vsoPos;
    float dist = abs(viewSpacePos.z);
    vec3 fogColor = vec3(0.3, 0.4, 0.8);
    float fogNear = 1900.0;
    float fogFar  = 11200.0;
    float fogFactor = clamp((fogFar - dist) / (fogFar - fogNear), 0.0, 1.0);
    //spec
    vec4 Lpv = view * Lp;
    vec3 lightDir = normalize(Lpv.xyz - viewSpacePos.xyz);
    vec3 viewDir = normalize(-viewSpacePos.xyz); // caméra en 0
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    float specular = spec * 0.5;
//
  vec4 texColor;
  vec3 Ld = normalize(gsoModPos - Lp).xyz;
  float phongIL = clamp(dot(-Ld, fragNormal), 0.0, 1.0);
  phongIL = phongIL * 0.8 + specular + 0.3;

    if(fragBlockType == 1){
        texColor = texture(cobble_tex, fragTexCoord);
        fragColor = vec4((phongIL * texColor.rgb), 1.0);
    }
    else if(fragBlockType == 2){
        texColor = texture(water_tex, fragTexCoord);
        fragColor = vec4((phongIL * texColor.rgb), 1.0);
    }
    else if(fragBlockType == 3){
        texColor = texture(grass_tex, fragTexCoord);
        fragColor = vec4((phongIL * texColor.rgb), 1.0);
    }
    else if(fragBlockType == 4){
        texColor = vec4(1.0);
        fragColor = vec4((phongIL * texColor.rgb), 1.0);
    }
    else if(fragBlockType == 5){
        texColor = vec4(0.0);
        fragColor = vec4((phongIL * texColor.rgb), 1.0);
    }
    // Mélange entre la couleur du fragment et la couleur du brouillard
    vec3 finalColor = mix(fogColor, fragColor.rgb, fogFactor);
    fragColor = vec4(finalColor, fragColor.a);
}

#version 450

in flat uint frag_id[1];

out vec4 fragColor;

uniform int map_width;
uniform int map_height;
uniform int map_depth;
uniform mat4 MVP;

void main(void) {
  if(frag_id[0] == 0){
    discard;
    //fragColor = vec4(0.0, 0.0, 0.0, 0.0);
  }
  else{
    fragColor = vec4(1.0, 1.0, 1.0, 0.05);
  }
}

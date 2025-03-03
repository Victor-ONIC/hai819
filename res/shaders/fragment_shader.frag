#version 330 core
out vec4 out_couleur_fragment;

in vec2 tex_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // out_couleur_fragment = vec4(1.0f, 0.5f, 0.2f, 1.0f);

    vec4 tex1 = texture(texture1, tex_coord);

    vec4 tex2 = texture(texture2, tex_coord);

    out_couleur_fragment = mix(tex1, tex2, 0.2f);
}

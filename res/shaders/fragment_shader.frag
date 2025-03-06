#version 330 core
#define M_PI 3.1415926535897932384626433832795
#define ONE 0.00390625
#define ONEHALF 0.001953125
out vec4 out_couleur_fragment;

in vec2 tex_coord;

uniform sampler2D permTexture;
//uniform sampler2D gradTexture;

float fade(float t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0); // Improved fade, yields C2-continuous noise
}

/*
 * 2D classic Perlin noise. Fast, but less useful than 3D noise.
 */
float noise(vec2 P)
{
  vec2 Pi = ONE*floor(P)+ONEHALF; // Integer part, scaled and offset for texture lookup
  vec2 Pf = fract(P);             // Fractional part for interpolation

  // Noise contribution from lower left corner
  vec2 grad00 = texture(permTexture, Pi).rg * 4.0 - 1.0;
  float n00 = dot(grad00, Pf);

  // Noise contribution from lower right corner
  vec2 grad10 = texture(permTexture, Pi + vec2(ONE, 0.0)).rg * 4.0 - 1.0;
  float n10 = dot(grad10, Pf - vec2(1.0, 0.0));

  // Noise contribution from upper left corner
  vec2 grad01 = texture(permTexture, Pi + vec2(0.0, ONE)).rg * 4.0 - 1.0;
  float n01 = dot(grad01, Pf - vec2(0.0, 1.0));

  // Noise contribution from upper right corner
  vec2 grad11 = texture(permTexture, Pi + vec2(ONE, ONE)).rg * 4.0 - 1.0;
  float n11 = dot(grad11, Pf - vec2(1.0, 1.0));

  // Blend contributions along x
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade(Pf.x));

  // Blend contributions along y
  float n_xy = mix(n_x.x, n_x.y, fade(Pf.y));

  // We're done, return the final noise value.
  //return n_xy;
  n_xy = mix(n_x.x, n_x.y, fade(Pf.y));
  return pow(abs(n_xy), 0.75); // Réduit les zones plates
}

void main()
{
    float n = 0.0, A = 1.0, mf = 1.0;
    float zoom = 1.0;
    vec4 color;
    //vec4 tex1 = texture(texture1, tex_coord);
    for(int i = 0; i < 5; ++i) {
        n += A * noise(zoom * mf * tex_coord);
        A = A / 2.0;
        mf = mf * 2.0;
    }
    //n = 1.0 - n;// Pour avoir des courbes concaves plutôt que convexes
    color = vec4(n, n, n, 1.0);
    //color = texture(permTexture, tex_coord);
    //color = vec4(1.0, 0.0, 0.0, 1.0);
    out_couleur_fragment = color;
}

#version 450
/* Le fragment shader est directement en relation avec les sorties du vertex shader */

/* Le fragment shader est directement en relation avec les sorties du vertex shader */
/* sortie du frament shader : une couleur */
in  vec4 vsoColor;
out vec4 fragColor;

void main(void) {
  /* la couleur de sortie utilise vsoColor linéairement interpolé au
     fragment (voir attribut flat pour désactiver l'interpolation). On
     utilise aussi la texture. On mélange les deux, voir la fonction
     GLSL mix. */
  fragColor = vec4(0.4, 0.0, 0.0, 1.0);
}

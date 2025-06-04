#version 330

out vec4 pixelColor;

// Dane z vertex shadera
in vec4 i_color;

// Uniformy
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec4 color;

void main(void) {
    // Pozycja aktualnego fragmentu w przestrzeni œwiata (nie mamy jej, wiêc uproszczenie)
    // Zak³adamy, ¿e normalnie by³aby przekazywana jako interpolowana wartoœæ, np. `fragPos`
    // Tu uproœcimy: przyjmujemy, ¿e fragmenty s¹ blisko kamery
    vec3 fragPos = vec3(0.0, 0.0, 0.0); // <- Najlepiej przekazaæ realn¹ pozycjê fragmentu!

    float distance = length(lightPos - fragPos);

    // Parametry os³abienia œwiat³a
    float constant = 1.0;
    float linear = 0.22;
    float quadratic = 0.20;

    float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);

    vec3 baseColor = i_color.rgb * attenuation;

    pixelColor = vec4(baseColor, color.a);
}

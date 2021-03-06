#ifndef QUARKGL_STANDARD_LIGHTS_FRAG_
#define QUARKGL_STANDARD_LIGHTS_FRAG_
#pragma qrk_include < lighting.frag >

#ifndef QRK_MAX_DIRECTIONAL_LIGHTS
#define QRK_MAX_DIRECTIONAL_LIGHTS 10
#endif
uniform QrkDirectionalLight qrk_directionalLights[QRK_MAX_DIRECTIONAL_LIGHTS];
uniform int qrk_directionalLightCount;

#ifndef QRK_MAX_POINT_LIGHTS
#define QRK_MAX_POINT_LIGHTS 10
#endif
uniform QrkPointLight qrk_pointLights[QRK_MAX_POINT_LIGHTS];
uniform int qrk_pointLightCount;

#ifndef QRK_MAX_SPOT_LIGHTS
#define QRK_MAX_SPOT_LIGHTS 10
#endif
uniform QrkSpotLight qrk_spotLights[QRK_MAX_SPOT_LIGHTS];
uniform int qrk_spotLightCount;

/** Calculate shading from all active directional lights. */
vec3 qrk_shadeAllDirectionalLights(QrkMaterial material, vec3 fragPos,
                                   vec3 normal, vec2 texCoords) {
  vec3 result = vec3(0.0);
  for (int i = 0; i < qrk_directionalLightCount; i++) {
    result += qrk_shadeDirectionalLight(material, qrk_directionalLights[i],
                                        fragPos, normal, texCoords);
  }
  return result;
}

/** Calculate shading from all active point lights. */
vec3 qrk_shadeAllPointLights(QrkMaterial material, vec3 fragPos, vec3 normal,
                             vec2 texCoords) {
  vec3 result = vec3(0.0);
  for (int i = 0; i < qrk_pointLightCount; i++) {
    result += qrk_shadePointLight(material, qrk_pointLights[i], fragPos, normal,
                                  texCoords);
  }
  return result;
}

/** Calculate shading from all active spot lights. */
vec3 qrk_shadeAllSpotLights(QrkMaterial material, vec3 fragPos, vec3 normal,
                            vec2 texCoords) {
  vec3 result = vec3(0.0);
  for (int i = 0; i < qrk_spotLightCount; i++) {
    result += qrk_shadeSpotLight(material, qrk_spotLights[i], fragPos, normal,
                                 texCoords);
  }
  return result;
}

/** Calculate shading from all light sources, except emission textures. */
vec3 qrk_shadeAllLights(QrkMaterial material, vec3 fragPos, vec3 normal,
                        vec2 texCoords) {
  vec3 directional =
      qrk_shadeAllDirectionalLights(material, fragPos, normal, texCoords);
  vec3 point = qrk_shadeAllPointLights(material, fragPos, normal, texCoords);
  vec3 spot = qrk_shadeAllSpotLights(material, fragPos, normal, texCoords);
  return directional + point + spot;
}

/**
 * Calculate a material's alpha based on its set of diffuse textures.
 * TODO: Research whether or not it makes sense to consider specular/emission
 * maps as well (I don't think it does).
 */
float qrk_materialAlpha(QrkMaterial material, vec2 texCoords) {
  float sum = 0.0;
  for (int i = 0; i < material.diffuseCount; i++) {
    sum += texture(material.diffuse[i], texCoords).a;
  }
  return min(sum, 1.0);
}

#endif

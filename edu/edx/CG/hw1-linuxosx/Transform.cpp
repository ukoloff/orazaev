// Transform.cpp: implementation of the Transform class.

#include <cstdio>

#include "Transform.h"


//Please implement the following functions:

void printMat3(const mat3& m) {
  printf("%.2f %.2f %.2f\n", m[0][0], m[0][1], m[0][2]);
  printf("%.2f %.2f %.2f\n", m[1][0], m[1][1], m[1][2]);
  printf("%.2f %.2f %.2f\n", m[2][0], m[2][1], m[2][2]);
}

void printMat4(const mat4& m) {
  printf("%.2f %.2f %.2f, %.2f\n", m[0][0], m[0][1], m[0][2], m[0][3]);
  printf("%.2f %.2f %.2f, %.2f\n", m[1][0], m[1][1], m[1][2], m[1][3]);
  printf("%.2f %.2f %.2f, %.2f\n", m[2][0], m[2][1], m[2][2], m[2][3]);
  printf("%.2f %.2f %.2f, %.2f\n", m[3][0], m[3][1], m[3][2], m[3][3]);
}

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW1 HERE
  mat3 rotationMatrix;

  mat3 eyeMatrix(
    1., 0., 0.,
    0., 1., 0.,
    0., 0., 1.
  );

  vec3 norm = glm::normalize(axis);

  mat3 crossProduct(
    0, norm[2], -norm[1],
    -norm[2], 0, norm[0],
    norm[1], -norm[0], 0
  );


  mat3 tenzorProduct(
    norm[0] * norm[0], norm[0] * norm[1], norm[0] * norm[2],
    norm[1] * norm[0], norm[1] * norm[1], norm[1] * norm[2],
    norm[2] * norm[0], norm[2] * norm[1], norm[2] * norm[2]
  );

  float theta = degrees * pi / 180.;

  rotationMatrix = eyeMatrix * cos(theta) + tenzorProduct * (1 - cos(theta)) +
                   crossProduct * sin(theta);

  return rotationMatrix;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE
  printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));
  eye = rotate(degrees, up) * eye;
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE 
  printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));
  vec3 v = glm::cross(eye, up);
  eye = rotate(degrees, v) * eye;
  up = rotate(degrees, v) * up;
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
  // YOUR CODE FOR HW1 HERE
  mat4 translation(
    1, 0, 0, -eye[0],
    0, 1, 0, -eye[1],
    0, 0, 1, -eye[2],
    0, 0, 0, 1
  );

  vec3 w = glm::normalize(eye);
  vec3 u = glm::normalize(glm::cross(up, w));
  vec3 v = glm::normalize(glm::cross(w, u));
  mat4 rotation(
    u[0], u[1], u[2], 0,
    v[0], v[1], v[2], 0,
    w[0], w[1], w[2], 0,
    0, 0, 0, 1
  );


  return translation * rotation;

  // Similar result
  // Here just for learning
  mat4 result(
    u[0], u[1], u[2], glm::dot(u, -eye),
    v[0], v[1], v[2], glm::dot(v, -eye),
    w[0], w[1], w[2], glm::dot(w, -eye),
    0, 0, 0, 1
  );

  return result;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}

#include "transformations.h"

#include <math.h>

void translate(double x, double y, double z, double *vertexes, int size) {
  if (x != 0) {
    for (int i = 0; i < size; i += 3) {
      vertexes[i] += x;
    }
  }
  if (y != 0) {
    for (int i = 1; i < size; i += 3) {
      vertexes[i] += y;
    }
  }
  if (z != 0) {
    for (int i = 2; i < size; i += 3) {
      vertexes[i] += z;
    }
  }
}

void translateX(double *prev_x, double curr_x, double *prev_y, double curr_y,
                double *prev_z, double curr_z, double *vertexes, int size) {
  translate(curr_x - *prev_x, curr_y - *prev_y, curr_z - *prev_z, vertexes,
            size);
  *prev_x = curr_x;
  *prev_y = curr_y;
  *prev_z = curr_z;
}

void rotate(double x, double y, double z, double *vertexes, int size) {
  double tmp1 = 0, tmp2 = 0;
  if (x != 0) {
    x = x * M_PI / 180;
    double sinx = sin(x);
    double cosx = cos(x);
    for (int i = 0; i < size; i += 3) {
      tmp1 = vertexes[i + 1] * cosx - vertexes[i + 2] * sinx; /* y */
      tmp2 = vertexes[i + 1] * sinx + vertexes[i + 2] * cosx; /* z */
      vertexes[i + 1] = tmp1;
      vertexes[i + 2] = tmp2;
    }
  }
  if (y != 0) {
    y = y * M_PI / 180;
    double siny = sin(y);
    double cosy = cos(y);
    for (int i = 0; i < size; i += 3) {
      tmp1 = vertexes[i] * cosy + vertexes[i + 2] * siny; /* x */
      tmp2 = vertexes[i + 2] * cosy - vertexes[i] * siny; /* z */
      vertexes[i] = tmp1;
      vertexes[i + 2] = tmp2;
    }
  }
  if (z != 0) {
    z = z * M_PI / 180;
    double sinz = sin(z);
    double cosz = cos(z);
    for (int i = 0; i < size; i += 3) {
      tmp1 = vertexes[i] * cosz - vertexes[i + 1] * sinz; /* x */
      tmp2 = vertexes[i] * sinz + vertexes[i + 1] * cosz; /* y */
      vertexes[i] = tmp1;
      vertexes[i + 1] = tmp2;
    }
  }
}

void rotateX(double *prev_x, double curr_x, double *prev_y, double curr_y,
             double *prev_z, double curr_z, double *vertexes, int size) {
  rotate(0, 0, -*prev_z, vertexes, size);  // 0?
  rotate(0, -*prev_y, 0, vertexes, size);  // 0?
  rotate(-*prev_x, 0, 0, vertexes, size);  // 0?
  rotate(curr_x, 0, 0, vertexes, size);
  rotate(0, curr_y, 0, vertexes, size);
  rotate(0, 0, curr_z, vertexes, size);  // new angle
  *prev_x = curr_x;
  *prev_y = curr_y;
  *prev_z = curr_z;
}

void scale(double x, double y, double z, double *vertexes, int size) {
  if (x != 0) {
    for (int i = 0; i < size; i += 3) {
      vertexes[i] *= x;
    }
  }
  if (y != 0) {
    for (int i = 1; i < size; i += 3) {
      vertexes[i] *= y;
    }
  }
  if (z != 0) {
    for (int i = 2; i < size; i += 3) {
      vertexes[i] *= z;
    }
  }
}

void scaleX(double *prev_scale, double *curr_scale, double *vertexes,
            int size) {
  if ((*prev_scale != *curr_scale) && (*curr_scale != 0)) {
    for (int i = 0; i < size; i++) {
      vertexes[i] = (vertexes[i] / *prev_scale) * *curr_scale;
    }
    *prev_scale = *curr_scale;
  }
}

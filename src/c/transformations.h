#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

/*move, rotate, scale*/

void translate(double x, double y, double z, double *vertexes, int size);
void rotate(double x, double y, double z, double *vertexes, int size);
void scale(double x, double y, double z, double *vertexes, int size);
void translateX(double *prev_x, double curr_x, double *prev_y, double curr_y,
                double *prev_z, double curr_z, double *vertexes, int size);
void scaleX(double *prev_scale, double *curr_scale, double *vertexes, int size);
void rotateX(double *prev_x, double curr_x, double *prev_y, double curr_y,
             double *prev_z, double curr_z, double *vertexes, int size);
#endif  // TRANSFORMATIONS_H

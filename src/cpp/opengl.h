#ifndef OPENGL_H
#define OPENGL_H
#include <QColorDialog>
#include <QOpenGLWidget>
#include <QWidget>
#include <QtOpenGL>
#include <iostream>

extern "C" {
#include "../c/parse_obj.h"
#include "../c/transformations.h"
}

class Opengl : public QOpenGLWidget {
  Q_OBJECT

 public:
  Opengl(QWidget *parent = 0);
  void initializeGL() override;
  void paintGL() override;
  void drawFigure();
  void resizeGL(int w, int h) override;
  void displayMethodSettings();
  void lineSettings();

 public:
  unsigned int num_of_vertexes = 0;
  unsigned int num_of_edges = 0;
  double *vertexes = 0;
  int *edges = 0;
  float lineWidth = 1.0;  // 1
  float pointSize = 1.0;
  int isDashed = 0;       // 0 - сплошная, 1 - пунктирная
  int displayMethod = 0;  // 0 - круглый, 1 - квадратныйб 2 - none
  int typeProjection = 0;
  coordinates_t coord = {-1, 1, -1, 1, -1, 1};
  QColor back_color = QColor::fromRgbF(0, 0, 0, 1);
  QColor edge_color = QColor::fromRgbF(1, 1, 1, 1);
  QColor vertex_color = QColor::fromRgbF(1, 1, 1, 1);
};

#endif  // OPENGL_H

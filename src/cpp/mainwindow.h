#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "opengl.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QString filePath = "";
  unsigned int num_of_vertexes = 0;
  unsigned int num_of_edges = 0;
  double *vertexes = 0;
  int *edges = 0;
  char *file_path = {0};
  QColor color;
  double prev_scale = 1;
  double prev_x_move = 0;
  double prev_y_move = 0;
  double prev_z_move = 0;
  double prev_x_angle = 0;
  double prev_z_angle = 0;
  double prev_y_angle = 0;
  // variables for settings
  // QThread *thread;

 public slots:
  void create_gif();

 private slots:
  void on_pushButton_chooseFile_clicked();

  void on_pushButton_colorOfBackGround_clicked();

  void on_pushButton_colorOfEdges_clicked();

  void on_pushButton_colorOfVertices_clicked();

  void on_doubleSpinBox_thicknessEdges_valueChanged(double arg1);
  void onRadioButtonDashed(bool checked);
  void onRadioButtonSolid(bool checked);
  void onRadioButtonCentral(bool checked);
  void onRadioButtonParallel(bool checked);

  void on_doubleSpinBox_sizeVertices_valueChanged(double arg1);

  void on_comboBox_displayMethod_currentIndexChanged(int index);

  void on_pushButton_screenshot_clicked();

  void on_pushButton_rebuild_clicked();

  void on_pushButton_screencast_clicked();

  void read_config();

  void write_config();

  void set_ui_values(char *bg_color, char *edges_color, char *vertexes_color);

 private:
  Ui::MainWindow *ui;
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *e);
  virtual void wheelEvent(QWheelEvent *event);
  double X = 0, Y = 0, Z = 0, RX = 0, RY = 0, RZ = 0, Scale = 1, preX = 0,
         preY = 0;
  coordinates_t coordinates = {-1, 1, -1, 1, -1, 1};
  void start_second_Thread();
  int central_factor = 0;
};
#endif  // MAINWINDOW_H

#include "mainwindow.h"

#include <unistd.h>

#include <QBuffer>
#include <QColorDialog>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QImage>
#include <QMouseEvent>
#include <QMovie>
#include <QOpenGLWidget>
#include <QPixmap>
#include <QThread>
#include <QTimer>
#include <QWheelEvent>
#include <thread>

#include "QtTest/QTest"
#include "qgifimage.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowFlags(
      Qt::Dialog |
      Qt::MSWindowsFixedSizeDialogHint);  // для ограничения расширения окна
  QApplication::setWindowIcon(QIcon(":/ico/260835.ico"));  // иконка приложения
  connect(ui->radioButton_dashed, &QRadioButton::toggled, this,
          &MainWindow::onRadioButtonDashed);
  connect(ui->radioButton_solid, &QRadioButton::toggled, this,
          &MainWindow::onRadioButtonSolid);
  connect(ui->radioButton_central, &QRadioButton::toggled, this,
          &MainWindow::onRadioButtonCentral);
  connect(ui->radioButton_parallel, &QRadioButton::toggled, this,
          &MainWindow::onRadioButtonParallel);
  read_config();
}

void MainWindow::read_config() {
  QString path = qApp->applicationDirPath() + "/settings.conf";
  QByteArray ba = path.toLatin1();
  const char *filepath = ba.data();
  FILE *fp = fopen(filepath, "r");
  if (fp != NULL) {
    char *edges_color = (char *)calloc(10, sizeof(char));     //"#000000";
    char *vertexes_color = (char *)calloc(10, sizeof(char));  //"#000000";
    char *bg_color = (char *)calloc(10, sizeof(char));        //"#000000";
    int num_of_args_read = fscanf(
        fp, "%d %d %s %f %s %f %d %s", &(ui->openGLWidget->typeProjection),
        &(ui->openGLWidget->isDashed), edges_color,
        &(ui->openGLWidget->lineWidth), vertexes_color,
        &(ui->openGLWidget->pointSize), &(ui->openGLWidget->displayMethod),
        bg_color);  // check if all variables were read
    if (num_of_args_read == 8) {
      set_ui_values(bg_color, edges_color, vertexes_color);
    } else {
      fprintf(stderr, "%s\n", "invalid config");
    }
    fclose(fp);
    free(edges_color);
    free(vertexes_color);
    free(bg_color);
  } else {
    fclose(fp);
  }
}

void MainWindow::set_ui_values(char *bg_color, char *edges_color,
                               char *vertexes_color) {
  QColor color1 = QColor(bg_color);
  ui->openGLWidget->back_color = color1;
  ui->pushButton_colorOfBackGround->setStyleSheet(
      "background-color:" + color1.name() + "; border-radius: 10px;");
  color1 = QColor(edges_color);
  ui->openGLWidget->edge_color = color1;
  ui->pushButton_colorOfEdges->setStyleSheet(
      "background-color:" + color1.name() + "; border-radius: 10px;");
  color1 = QColor(vertexes_color);
  ui->openGLWidget->vertex_color = color1;
  ui->pushButton_colorOfVertices->setStyleSheet(
      "background-color:" + color1.name() + "; border-radius: 10px;");
  if (ui->openGLWidget->typeProjection == 0) {
    ui->radioButton_parallel->setChecked(true);
    ui->radioButton_central->setChecked(false);
  } else {
    ui->radioButton_parallel->setChecked(false);
    ui->radioButton_central->setChecked(true);
  }
  if (ui->openGLWidget->isDashed == 0) {
    ui->radioButton_solid->setChecked(true);
    ui->radioButton_dashed->setChecked(false);
  } else {
    ui->radioButton_solid->setChecked(false);
    ui->radioButton_dashed->setChecked(true);
  }
  ui->doubleSpinBox_thicknessEdges->setValue(ui->openGLWidget->lineWidth);
  ui->doubleSpinBox_sizeVertices->setValue(ui->openGLWidget->pointSize);
  ui->comboBox_displayMethod->setCurrentIndex(ui->openGLWidget->displayMethod);
}

void MainWindow::write_config() {
  QString path = qApp->applicationDirPath() + "/settings.conf";
  QByteArray ba = path.toLatin1();
  const char *filepath = ba.data();
  QByteArray ba1 = ui->openGLWidget->back_color.name().toLatin1();
  const char *bg = ba1.data();
  QByteArray ba2 = ui->openGLWidget->vertex_color.name().toLatin1();
  const char *v_color = ba2.data();
  QByteArray ba3 = ui->openGLWidget->edge_color.name().toLatin1();
  const char *e_color = ba3.data();
  FILE *fp = fopen(filepath, "w");
  fprintf(fp, "%d %d %s %f %s %f %d %s\n", ui->openGLWidget->typeProjection,
          ui->openGLWidget->isDashed, e_color, ui->openGLWidget->lineWidth,
          v_color, ui->openGLWidget->pointSize, ui->openGLWidget->displayMethod,
          bg);
  fclose(fp);
}

MainWindow::~MainWindow() {
  write_config();
  if (vertexes != NULL) {
    free(vertexes);
  }
  if (edges != NULL) {
    free(edges);
  }
  delete ui;
}

void MainWindow::on_pushButton_chooseFile_clicked() {
  this->filePath = QFileDialog::getOpenFileName(
      this, "Choose file", "../../", "All Files (*.*);; OBJ Files (*.obj);");
  QFileInfo fileinfo(this->filePath);
  ui->label_setLocation->setText(
      this->filePath);  // заполнение поля location в info
  ui->lineEdit_fileName->setText(
      fileinfo.fileName());  // заполнение поля filename в info
  ui->lineEdit_fileSize->setText(
      QString::number((double)fileinfo.size() / 1000000, 'f',
                      3));  // заполнение поля fileSize в info

  QByteArray ba = filePath.toLocal8Bit();  // преобразование переменной filePath
                                           // из QString в char* file_path
  file_path = ba.data();
  if (ui->lineEdit_fileName->text() != "") {
    free(vertexes);
    free(edges);
  }
  parse_obj(file_path, &num_of_vertexes, &num_of_edges, &vertexes, &edges,
            &coordinates);  // работа бэка
  prev_scale = 1;
  prev_x_angle = 0;
  prev_y_angle = 0;
  prev_z_angle = 0;

  ui->openGLWidget->coord = coordinates;

  ui->lineEdit_numbEdges->setText(QString::number(
      num_of_edges));  // запонение полей edges и vertices в info
  ui->lineEdit_numbVertices->setText(QString::number(num_of_vertexes));

  ui->openGLWidget->num_of_vertexes =
      num_of_vertexes;  // передаём в класс OpenGL все значения
  ui->openGLWidget->num_of_edges = num_of_edges;
  ui->openGLWidget->vertexes = vertexes;
  ui->openGLWidget->edges = edges;

  ui->openGLWidget->initializeGL();  // вызов методов отрисовки
  ui->openGLWidget->resizeGL(ui->openGLWidget->width(),
                             ui->openGLWidget->height());
  ui->openGLWidget->paintGL();
}

void MainWindow::on_pushButton_colorOfBackGround_clicked() {  // изменение цвета
                                                              // бэкграунда
  color = QColorDialog::getColor(Qt::black, this);
  if (color.isValid()) {
    ui->openGLWidget->back_color = color;
    ui->pushButton_colorOfBackGround->setStyleSheet(
        "background-color:" + color.name() + "; border-radius: 10px;");
  }
}

void MainWindow::on_pushButton_colorOfEdges_clicked() {  // изменение цвета
                                                         // граней
  color = QColorDialog::getColor(Qt::white, this);
  if (color.isValid()) {
    ui->openGLWidget->edge_color = color;
    ui->pushButton_colorOfEdges->setStyleSheet(
        "background-color:" + color.name() + "; border-radius: 10px;");
  }
}

void MainWindow::on_pushButton_colorOfVertices_clicked() {  // изменение цвета
                                                            // точек узлов
  color = QColorDialog::getColor(Qt::white, this);
  if (color.isValid()) {
    ui->openGLWidget->vertex_color = color;
    ui->pushButton_colorOfVertices->setStyleSheet(
        "background-color:" + color.name() + "; border-radius: 10px;");
  }
}

void MainWindow::on_doubleSpinBox_thicknessEdges_valueChanged(
    double arg1) {  // изменение толщины граней
  ui->openGLWidget->lineWidth = arg1;
}
void MainWindow::onRadioButtonDashed(
    bool checked) {  // нажатие радиокнопки пунктира
  if (checked) {
    ui->openGLWidget->isDashed = 1;
  }
}
void MainWindow::onRadioButtonSolid(
    bool checked) {  // нажатие радиокнопки сплошной линии
  if (checked) {
    ui->openGLWidget->isDashed = 0;
  }
}
void MainWindow::onRadioButtonCentral(
    bool checked) {  // нажатие радиокнопки метода отображения Central
  if (checked) {
    ui->openGLWidget->typeProjection = 1;
    central_factor = coordinates.Zmax +
                     10;  // долно быть смещение по оси Z, чтобы полностью вошло
    printf("%d", central_factor);
    translateX(&prev_x_move, ui->doubleSpinBox_OX->value(), &prev_y_move,
               ui->doubleSpinBox_OY->value(), &prev_z_move,
               ui->doubleSpinBox_OZ->value() - central_factor, vertexes,
               num_of_vertexes * 3);
  }
}
void MainWindow::onRadioButtonParallel(
    bool checked) {  // нажатие радиокнопки метода отображения Parallel
  if (checked) {
    ui->openGLWidget->typeProjection = 0;
    central_factor = 0;
    translateX(&prev_x_move, 0, &prev_y_move, 0, &prev_z_move, 0, vertexes,
               num_of_vertexes * 3);
    ui->doubleSpinBox_OX->setValue(0);
    ui->doubleSpinBox_OY->setValue(0);
    ui->doubleSpinBox_OZ->setValue(0);
  }
}

void MainWindow::on_doubleSpinBox_sizeVertices_valueChanged(
    double arg1)  // изменение размера точек узлов
{
  ui->openGLWidget->pointSize = arg1;
}

void MainWindow::on_comboBox_displayMethod_currentIndexChanged(
    int index) {  // изменение метода отображения точек 0 - круглый, 1 -
                  // квадратный
  ui->openGLWidget->displayMethod = index;
}

void MainWindow::on_pushButton_screenshot_clicked() {  // скриншот
  QString nameOfCurrentScreenshot = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"), "/home",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  nameOfCurrentScreenshot += "/screenshot_";
  nameOfCurrentScreenshot +=
      QDateTime::currentDateTime().toString("dd.MM.yyyy_HH:mm:ss");
  if (ui->radioButton_bmp->isChecked())  // расширение скрина
    nameOfCurrentScreenshot += ".bmp";
  else if (ui->radioButton_jpeg->isChecked())
    nameOfCurrentScreenshot += ".jpeg";

  ui->openGLWidget->grab().save(nameOfCurrentScreenshot);  // сохранение скрина
}

void MainWindow::on_pushButton_rebuild_clicked()  //
{
  X = ui->doubleSpinBox_OX->value();  // сдвиг по осям
  Y = ui->doubleSpinBox_OY->value();
  Z = ui->doubleSpinBox_OZ->value() - central_factor;
  RX = ui->doubleSpinBox_ROX->value();  // вращение по осям
  RY = ui->doubleSpinBox_ROY->value();
  RZ = ui->doubleSpinBox_ROZ->value();
  Scale = ui->doubleSpinBox_scale->value();  // scale по осям
  if (prev_scale < Scale) {
    scaleX(&prev_scale, &Scale, vertexes, num_of_vertexes * 3);
  }
  translateX(&prev_x_move, X, &prev_y_move, Y, &prev_z_move, Z, vertexes,
             num_of_vertexes * 3);
  rotateX(&prev_x_angle, RX, &prev_y_angle, RY, &prev_z_angle, RZ, vertexes,
          num_of_vertexes * 3);
  if (prev_scale > Scale) {
    scaleX(&prev_scale, &Scale, vertexes, num_of_vertexes * 3);
  }
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
  if (e->button() == Qt::LeftButton) {
    preX = e->pos().x();  // предыдущие координаты мыши
    preY = e->pos().y();
  }
}

void MainWindow::mouseMoveEvent(
    QMouseEvent *e) {  // отслеживание поворотов мышкой по виджету

  if (e->pos().x() >= 22 && e->pos().x() <= 711 && e->pos().y() >= 22 &&
      e->pos().x() <= 684) {  // для координат виджета openGL
    RX = RX + (e->pos().y() - preY) / 4;
    ui->doubleSpinBox_ROX->setValue(RX);
    RY = RY + (e->pos().x() - preX) / 4;
    ui->doubleSpinBox_ROY->setValue(RY);
  }
  preX = e->pos().x();  // предыдущие координаты мыши
  preY = e->pos().y();
  // rotate(RX, RY, RZ, vertexes, num_of_vertexes * 3);
  rotateX(&prev_x_angle, RX, &prev_y_angle, RY, &prev_z_angle, RZ, vertexes,
          num_of_vertexes * 3);
  QMainWindow::mouseMoveEvent(e);
}

void MainWindow::wheelEvent(QWheelEvent *event) {
  // увеличение масштаба, когда колесо прокрутки находится далеко от
  // пользователя, уменьшение масштаба, когда колесо прокрутки поворачивается к
  // пользователю
  double scrole_scale = 0;
  if (event->position().x() >= 22 && event->position().x() <= 711 &&
      event->position().y() >= 22 &&
      event->position().x() <= 684) {  // для координат виджета openGL
    if (event->angleDelta().y() > 0 &&
        ui->doubleSpinBox_scale->value() < 20) {  // если увеличиваем
      scrole_scale = (ui->doubleSpinBox_scale->value()) + 0.1;
      scaleX(&prev_scale, &scrole_scale, vertexes, num_of_vertexes * 3);
      ui->doubleSpinBox_scale->setValue(scrole_scale);
    } else if (event->angleDelta().y() < 0 &&
               ui->doubleSpinBox_scale->value() > 0) {  // если уменьшаем
      scrole_scale = (ui->doubleSpinBox_scale->value()) - 0.1;
      scaleX(&prev_scale, &scrole_scale, vertexes, num_of_vertexes * 3);
      ui->doubleSpinBox_scale->setValue(scrole_scale);
    }
  }
}

void MainWindow::on_pushButton_screencast_clicked() {
  create_gif();
}


void MainWindow::create_gif() {
  QGifImage gif(QSize(640, 480));  // создание гифки
  gif.setDefaultDelay(100);
  QPixmap screenshot;
  QImage image;
  // плотности пикселей
  int xOffset = 0;  // координаты начала гифки в виджете
  int yOffset = 0;

  double sscale = 1;  // изменение scale
  double x_angle = 0;
  double y_angle = 0;
  double z_angle = 0;
  double x_move = 0;
  double y_move = 0;
  double z_move = 0;

  QString save_file_path = QFileDialog::getSaveFileName(
      this, "Choose file", "../../screencast.gif", "GIF Files (*.gif);");
  if (prev_scale < Scale) {
    scaleX(&prev_scale, &sscale, vertexes, num_of_vertexes * 3);
  }
  translateX(&prev_x_move, 0, &prev_y_move, 0, &prev_z_move, 0, vertexes,
             num_of_vertexes * 3);
  rotateX(&prev_x_angle, 0, &prev_y_angle, 0, &prev_z_angle, 0, vertexes,
          num_of_vertexes * 3);
  if (prev_scale > Scale) {
    scaleX(&prev_scale, &sscale, vertexes, num_of_vertexes * 3);
  }

  for (int i = 0; i < 50; i += 1) {
    screenshot = ui->openGLWidget->grab();  // скриншот
    // пикселей
    screenshot = screenshot.scaled(
        screenshot.width() / 2, screenshot.height() / 2, Qt::KeepAspectRatio,
        Qt::SmoothTransformation);  // scale / 2 + параметры сохранения
                                    // пропорций и сглаживания при
                                    // масштабировании (так как скриншот
                                    // делается в 2 раза больше, чем размер
                                    // виджета)

    xOffset = (screenshot.width() - 640) /
              2;  // расчёт точки начала для обрезки скриншота по центру
    yOffset = (screenshot.height() - 480) / 2;
    screenshot = screenshot.copy(xOffset, yOffset, 640, 480);  // обрезка
    image = screenshot.toImage();
    gif.addFrame(image, QPoint(0, 0));  // добавление скриншота в gif
    translateX(&prev_x_move, x_move + ui->doubleSpinBox_OX->value() / 50,
               &prev_y_move, y_move + ui->doubleSpinBox_OY->value() / 50,
               &prev_z_move,
               z_move + (ui->doubleSpinBox_OZ->value() - central_factor) / 50,
               vertexes, num_of_vertexes * 3);
    rotateX(&prev_x_angle, x_angle + ui->doubleSpinBox_ROX->value() / 50,
            &prev_y_angle, y_angle + ui->doubleSpinBox_ROY->value() / 50,
            &prev_z_angle, z_angle + ui->doubleSpinBox_ROZ->value() / 50,
            vertexes, num_of_vertexes * 3);
    x_move += ui->doubleSpinBox_OX->value() / 50;
    y_move += ui->doubleSpinBox_OY->value() / 50;
    z_move += ui->doubleSpinBox_OZ->value() / 50;
    x_angle += ui->doubleSpinBox_ROX->value() / 50;
    y_angle += ui->doubleSpinBox_ROY->value() / 50;
    z_angle += ui->doubleSpinBox_ROZ->value() / 50;
    if (ui->doubleSpinBox_scale->value() > 1) {
      sscale += (ui->doubleSpinBox_scale->value() - 1) / 50;
      scaleX(&prev_scale, &sscale, vertexes, num_of_vertexes * 3);

    } else if (ui->doubleSpinBox_scale->value() < 1) {
      sscale += -(ui->doubleSpinBox_scale->value()) / 50;
      scaleX(&prev_scale, &sscale, vertexes, num_of_vertexes * 3);
    }

  }

  gif.save(save_file_path);  // сохранение готовой гифки
}

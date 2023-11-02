#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  win = new glView(ui->openGLWidget);
  win->resize(1000, 1000);
  choiceDirectory = QApplication::applicationDirPath();
  fileList = new QFileSystemModel(this);
  fileList->setFilter(QDir::AllEntries | QDir::NoDot);
  ui->listView->setModel(fileList);
  ui->listView->setRootIndex(
      fileList->setRootPath(choiceDirectory.section("/", 0, -5)));
  settingConfig();
  connect(ui->pushButtonColorDot, SIGNAL(clicked()), SLOT(setColor()));
  connect(ui->pushButtonColorLine, SIGNAL(clicked()), SLOT(setColor()));
  connect(ui->pushButtonColorBack, SIGNAL(clicked()), SLOT(setColor()));
  connect(ui->radioButtoCentralProection, SIGNAL(clicked()),
          SLOT(proectionChanged()));
  connect(ui->radioButtonParallelProection, SIGNAL(clicked()),
          SLOT(proectionChanged()));
  connect(ui->radioButtonDotCircle, SIGNAL(clicked()), SLOT(dotTypeChanged()));
  connect(ui->radioButtonDotSquare, SIGNAL(clicked()), SLOT(dotTypeChanged()));
  connect(ui->radioButtonDotNothing, SIGNAL(clicked()), SLOT(dotTypeChanged()));
  connect(ui->radioButtonLineSolid, SIGNAL(clicked()), SLOT(lineTypeChanged()));
  connect(ui->radioButtonLineDotted, SIGNAL(clicked()),
          SLOT(lineTypeChanged()));
  connect(ui->radioButtonLineNothing, SIGNAL(clicked()),
          SLOT(lineTypeChanged()));
  connect(ui->radioButtonRotationX, SIGNAL(clicked()),
          SLOT(radioButtonValue()));
  connect(ui->radioButtonRotationY, SIGNAL(clicked()),
          SLOT(radioButtonValue()));
  connect(ui->radioButtonRotationZ, SIGNAL(clicked()),
          SLOT(radioButtonValue()));
  connect(ui->radioButtonMoveX, SIGNAL(clicked()), SLOT(radioButtonValue()));
  connect(ui->radioButtonMoveY, SIGNAL(clicked()), SLOT(radioButtonValue()));
  connect(ui->radioButtonMoveZ, SIGNAL(clicked()), SLOT(radioButtonValue()));
  connect(ui->radioButtonZoomeLine, SIGNAL(clicked()),
          SLOT(radioButtonValue()));
  connect(ui->radioButtonZoomeDot, SIGNAL(clicked()), SLOT(radioButtonValue()));
  connect(ui->radioButtonZoomeModel, SIGNAL(clicked()),
          SLOT(radioButtonValue()));
  imagesCounter = 0;
  timer = new QTimer();
  images = new QImage[50];
  connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerEnd()));

  setStyles();
}

MainWindow::~MainWindow() {
  win->setConfig();
  win->freeModel();
  delete fileList;
  delete win;
  //    delete timer;
  delete[] images;
  delete ui;
}

void MainWindow::settingConfig() {
  QString dirPath = QDir::home().absolutePath() + "/Desktop/config.txt";
  get_config(&set, dirPath.toStdString().c_str());
  if (set.vertex_type == 0) {
    ui->radioButtonDotCircle->setChecked(1);
  } else if (set.vertex_type == 1) {
    ui->radioButtonDotSquare->setChecked(1);
  } else if (set.vertex_type == 2) {
    ui->radioButtonDotNothing->setChecked(1);
  }
  if (set.rib_type == 0) {
    ui->radioButtonLineSolid->setChecked(1);
  } else if (set.rib_type == 1) {
    ui->radioButtonLineDotted->setChecked(1);
  } else if (set.rib_type == 2) {
    ui->radioButtonLineNothing->setChecked(1);
  }
  if (set.projection_type == 0) {
    ui->radioButtonParallelProection->setChecked(1);
  } else if (set.projection_type == 1) {
    ui->radioButtoCentralProection->setChecked(1);
  }
  ui->labelColorLine->setStyleSheet(
      QString("background-color: %1").arg(win->getColorLine().name()));
  ui->labelColorDot->setStyleSheet(
      QString("background-color: %1").arg(win->getColorDot().name()));
  ui->labelColorBack->setStyleSheet(
      QString("background-color: %1").arg(win->getColorBack().name()));
  valueZoomeDot = set.vertex_size;
  valueZoomeLine = set.rib_thickness;
}

void MainWindow::proectionChanged() {
  if (ui->radioButtonParallelProection->isChecked()) index = 0;
  if (ui->radioButtoCentralProection->isChecked()) index = 1;
  win->IndexChanged(index);
}

void MainWindow::dotTypeChanged() {
  if (ui->radioButtonDotCircle->isChecked()) {
    index = 0;
  } else if (ui->radioButtonDotSquare->isChecked()) {
    index = 1;
  } else {
    index = 2;
  }
  win->DotTypeChanged(index);
}

void MainWindow::lineTypeChanged() {
  if (ui->radioButtonLineSolid->isChecked()) {
    index = 0;
  } else if (ui->radioButtonLineDotted->isChecked()) {
    index = 1;
  } else if (ui->radioButtonLineNothing->isChecked()) {
    index = 2;
  }
  win->LineTypeChanged(index);
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index) {
  QFileInfo fileInfo = fileList->fileInfo(index);
  if (fileInfo.fileName() == "..") {
    dir = fileInfo.dir();
    dir.cdUp();
    ui->listView->setRootIndex(fileList->index(dir.absolutePath()));
  } else if (fileInfo.fileName() == ".") {
    ui->listView->setRootIndex(fileList->index(""));
  } else if (fileInfo.isDir()) {
    ui->listView->setRootIndex(index);
  } else if (fileInfo.completeSuffix() == "obj") {
    QString filePath = fileInfo.filePath();
    ui->lineEdit->setText(filePath);
  } else {
    ui->statusBar->showMessage("⛔️ INPUT ERROR", 3000);
  }
}

void MainWindow::on_pushButton_clicked() {
  int erorr = parser_vertex(&points, &model,
                            ui->lineEdit->text().toStdString().c_str());
  if (erorr == 1) {
    ui->statusBar->showMessage(
        "⛔️ Ошибка открытия файла", 3000);
  } else if (erorr == 2) {
    ui->statusBar->showMessage(
        "⛔️ Некорректные координаты вершин в "
        "файле",
        3000);
  } else if (erorr == 3) {
    ui->statusBar->showMessage(
        "⛔️ Некорректные полигоны в файле", 3000);
  } else if (erorr == 4) {
    ui->statusBar->showMessage(
        "⛔️ Обращение к нулевой вершине", 3000);
  } else {
    win->translate(points, model);
    ui->statusBar->showMessage(
        QString("Количество Вершин: %1      Количество Ребер: %2")
            .arg(points.amountV / 3)
            .arg(model.amountF / 2));
  }
  free_all_struct(&points, &model);
}

void MainWindow::setColor() {
  color = QColorDialog::getColor();
  if (color.isValid()) {
    QObject *obj = (QObject *)(sender());
    if (obj->objectName() == "pushButtonColorDot") {
      win->setColorDot(color);
      ui->labelColorDot->setStyleSheet(
          QString("background-color: %1").arg(color.name()));
    } else if (obj->objectName() == "pushButtonColorLine") {
      win->setColorLine(color);
      ui->labelColorLine->setStyleSheet(
          QString("background-color: %1").arg(color.name()));
    } else if (obj->objectName() == "pushButtonColorBack") {
      win->setColorBack(color);
      ui->labelColorBack->setStyleSheet(
          QString("background-color: %1").arg(color.name()));
    }
  }
}

void MainWindow::radioButtonValue() {
  QObject *obj = (QObject *)(sender());
  if (obj->objectName() == "radioButtonRotationX") {
    ui->verticalScrollBar->setValue(valueRotationX);
    win->scrollBar(valueRotationX, obj->objectName());
  }
  if (obj->objectName() == "radioButtonRotationY") {
    ui->verticalScrollBar->setValue(valueRotationY);
    win->scrollBar(valueRotationY, obj->objectName());
  }
  if (obj->objectName() == "radioButtonRotationZ") {
    ui->verticalScrollBar->setValue(valueRotationZ);
    win->scrollBar(valueRotationZ, obj->objectName());
  }
  if (obj->objectName() == "radioButtonMoveX") {
    ui->verticalScrollBar->setValue(valueMoveX);
    win->scrollBar(valueMoveX, obj->objectName());
  }
  if (obj->objectName() == "radioButtonMoveY") {
    ui->verticalScrollBar->setValue(valueMoveY);
    win->scrollBar(valueMoveY, obj->objectName());
  }
  if (obj->objectName() == "radioButtonMoveZ") {
    ui->verticalScrollBar->setValue(valueMoveZ);
    win->scrollBar(valueMoveZ, obj->objectName());
  }
  if (obj->objectName() == "radioButtonZoomeLine") {
    ui->verticalScrollBar->setValue(valueZoomeLine);
    win->scrollBar(valueZoomeLine, obj->objectName());
    //        qDebug()<<"L"<<valueZoomeLine;
  }
  if (obj->objectName() == "radioButtonZoomeDot") {
    ui->verticalScrollBar->setValue(valueZoomeDot);
    //        qDebug()<<"D"<<valueZoomeDot;
    win->scrollBar(valueZoomeDot, obj->objectName());
  }
  if (obj->objectName() == "radioButtonZoomeModel") {
    ui->verticalScrollBar->setValue(valueZoomeModel);
    win->scrollBar(valueZoomeModel, obj->objectName());
  }
}

void MainWindow::on_verticalScrollBar_valueChanged(int value) {
  if (ui->radioButtonRotationX->isChecked()) {
    valueRotationX = value;
    checkerName = ui->radioButtonRotationX->objectName();
  }
  if (ui->radioButtonRotationY->isChecked()) {
    valueRotationY = value;
    checkerName = ui->radioButtonRotationY->objectName();
  }
  if (ui->radioButtonRotationZ->isChecked()) {
    valueRotationZ = value;
    checkerName = ui->radioButtonRotationZ->objectName();
  }
  if (ui->radioButtonMoveX->isChecked()) {
    valueMoveX = value;
    checkerName = ui->radioButtonMoveX->objectName();
  }
  if (ui->radioButtonMoveY->isChecked()) {
    valueMoveY = value;
    checkerName = ui->radioButtonMoveY->objectName();
  }
  if (ui->radioButtonMoveZ->isChecked()) {
    valueMoveZ = value;
    checkerName = ui->radioButtonMoveZ->objectName();
  }
  if (ui->radioButtonZoomeLine->isChecked()) {
    valueZoomeLine = value;
    checkerName = ui->radioButtonZoomeLine->objectName();
  }
  if (ui->radioButtonZoomeDot->isChecked()) {
    valueZoomeDot = value;
    checkerName = ui->radioButtonZoomeDot->objectName();
  }
  if (ui->radioButtonZoomeModel->isChecked()) {
    valueZoomeModel = value;
    checkerName = ui->radioButtonZoomeModel->objectName();
  }
  win->scrollBar(value, checkerName);
}

void MainWindow::on_pushButtonRec_clicked() {
  if (ui->comboBoxFormat->currentText() == ".jpg") {
    QDateTime curTime = QDateTime::currentDateTime();
    QImage img = win->grabFramebuffer();
    QString formatDate = "hh:mm:ss_dd.MM.yyyy";
    QString dirPath = QDir::home().absolutePath() + "/Desktop/images/";
    if (QDir().exists(dirPath)) {
      img.save(dirPath + curTime.toString(formatDate) + ".jpg");
    } else {
      QDir().mkpath(dirPath);
      img.save(dirPath + curTime.toString(formatDate) + ".jpg");
    }
  } else if (ui->comboBoxFormat->currentText() == ".bmp") {
    QDateTime curTime = QDateTime::currentDateTime();
    QImage img = win->grabFramebuffer();
    QString formatDate = "hh:mm:ss_dd.MM.yyyy";
    QString dirPath = QDir::home().absolutePath() + "/Desktop/images/";
    if (QDir().exists(dirPath)) {
      img.save(dirPath + curTime.toString(formatDate) + ".bmp");
    } else {
      QDir().mkpath(dirPath);
      img.save(dirPath + curTime.toString(formatDate) + ".bmp");
    }
  } else if (ui->comboBoxFormat->currentText() == ".gif") {
    qDebug() << "Create gif " << QDateTime::currentDateTime();
    ui->pushButtonRec->setEnabled(false);
    timer->start(100);
  }
}

void MainWindow::slotTimerEnd() {
  if (imagesCounter < 50) {
    images[imagesCounter] = win->grabFramebuffer();
    imagesCounter++;
  } else {
    timer->stop();
    QGifImage gif(QSize(640, 480));
    gif.setDefaultDelay(100);
    gif.setDefaultTransparentColor(Qt::yellow);
    for (int i = 0; i < 50; ++i) {
      gif.addFrame(images[i].scaled(640, 480));
    }
    QDateTime curTime = QDateTime::currentDateTime();
    QImage img = win->grabFramebuffer();
    QString formatDate = "hh:mm:ss_dd.MM.yyyy";
    QString dirPath = QDir::home().absolutePath() + "/Desktop/images/";
    if (QDir().exists(dirPath)) {
      gif.save(dirPath + curTime.toString(formatDate) + ".gif");
    } else {
      QDir().mkpath(dirPath);
      gif.save(dirPath + curTime.toString(formatDate) + ".gif");
    }
    imagesCounter = 0;
    ui->pushButtonRec->setEnabled(true);
    qDebug() << "Finish create gif " << QDateTime::currentDateTime();
  }
}

void MainWindow::setStyles() {
  ui->centralwidget->setStyleSheet(
      "background-color: #D1B3EA;"
      "");
  ui->lineEdit->setStyleSheet(
      "background-color: white; color: black; padding: 3px;"
      "border: 1px solid #CEE4F5;"
      "border-radius: 5px;");

  ui->listView->setStyleSheet(
      "background-color: white; color: black; padding: 10px;"
      "border: 1px solid #CEE4F5;"
      "border-radius: 5px;");

  ui->pushButtonColorBack->setStyleSheet(
      "QPushButton { background-color: #CEE4F5; color: black; border-radius: "
      "5px; }"
      "QPushButton:hover { background-color: white; color: black; }");
  ui->pushButtonColorDot->setStyleSheet(
      "QPushButton { background-color: #CEE4F5; color: black; border-radius: "
      "5px; }"
      "QPushButton:hover { background-color: white; color: black; }");
  ui->pushButtonColorLine->setStyleSheet(
      "QPushButton { background-color: #CEE4F5; color: black; border-radius: "
      "5px; }"
      "QPushButton:hover { background-color: white; color: black; }");
  ui->pushButton->setStyleSheet(
      "QPushButton { background-color: #CEE4F5; color: black;  border-radius: "
      "5px; }"
      "QPushButton:hover { background-color: white; color: black; }");
  ui->pushButtonRec->setStyleSheet(
      "QPushButton { background-color: #CEE4F5; color: black;  border-radius: "
      "5px; }"
      "QPushButton:hover { background-color: white; color: black; }"
      "QPushButton:disabled { background-color: #ECA9B9;}");

  ui->verticalScrollBar->setStyleSheet(
      "background-color: #EBA9B9;"
      "");

  ui->groupBox_5->setStyleSheet("background-color: #D1B3EA;;");
}

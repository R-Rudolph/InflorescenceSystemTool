#ifndef SEGMENTATIONTESTERWIDGET_H
#define SEGMENTATIONTESTERWIDGET_H

#include <QWidget>

class QFormLayout;
class QPushButton;
class ConvolutionalSegmenterParameterDialog;

class SegmentationTesterWidget : public QWidget
{
  Q_OBJECT
  QString* dataFile;

  QFormLayout* layout;
  QPushButton* configButton;
  QPushButton* startButton;
  ConvolutionalSegmenterParameterDialog* convDialog;
public:
  explicit SegmentationTesterWidget(ConvolutionalSegmenterParameterDialog* convSegDialog, QString* dataFile, QWidget *parent = nullptr);

signals:

private slots:
  void start();
};

#endif // SEGMENTATIONTESTERWIDGET_H

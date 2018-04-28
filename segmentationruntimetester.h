#ifndef SEGMENTATIONRUNTIMETESTER_H
#define SEGMENTATIONRUNTIMETESTER_H

#include <QWidget>

class ConvolutionalSegmenterParameterDialog;
class QPushButton;
class QFormLayout;

class SegmentationRuntimeTester : public QWidget
{
  Q_OBJECT
  int repeats;
  QString* dataFile;
  QFormLayout* layout;
  QPushButton* convSegConfigButton;
  QPushButton* startButton;
  ConvolutionalSegmenterParameterDialog* convSegDialog;
public:
  explicit SegmentationRuntimeTester(ConvolutionalSegmenterParameterDialog* convSegDialog, QString* dataFile, QWidget *parent = nullptr);

signals:

public slots:
  void start();
};

#endif // SEGMENTATIONRUNTIMETESTER_H

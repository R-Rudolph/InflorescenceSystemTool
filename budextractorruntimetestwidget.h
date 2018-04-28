#ifndef BUDEXTRACTORRUNTIMETESTWIDGET_H
#define BUDEXTRACTORRUNTIMETESTWIDGET_H

#include <QWidget>
#include <chrono>

class QFormLayout;
class QPushButton;
class ChtExtractorConfigDialog;
class QComboBox;
class ConvolutionalSegmenterParameterDialog;
class QDoubleSpinBox;
#include "extractortest/budextractortest.h"

class BudExtractorRuntimeTestWidget : public QWidget
{
  Q_OBJECT
  int repeat;
  BudExtractorTest budExtractorTest;
  QString* dataFile;
  QFormLayout* dataLayout;
  QComboBox* segmentationBox;
  ConvolutionalSegmenterParameterDialog* convolutionalSegmenterParameterDialog;
  ChtExtractorConfigDialog* configDialog;
  QPushButton* startButton;
  QPushButton* configButton;

  void saveSettings();
  void loadSettings();
public:
  explicit BudExtractorRuntimeTestWidget(ChtExtractorConfigDialog* chtDialog, ConvolutionalSegmenterParameterDialog* convSegDialog, QString *dataFile, QWidget *parent = nullptr);

signals:

private slots:
  void startButtonPressed();
};

#endif // BUDEXTRACTORRUNTIMETESTWIDGET_H

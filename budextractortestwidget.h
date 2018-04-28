#ifndef BUDEXTRACTORTESTWIDGET_H
#define BUDEXTRACTORTESTWIDGET_H

#include <QWidget>
#include <QMap>

#include "extractortest/budextractortest.h"

class QRadioButton;
class QButtonGroup;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QFormLayout;
class QLineEdit;
class FileDirSelectionWidget;
class ExtractorConfigDialog;
class QDoubleSpinBox;
class QComboBox;
class ConvolutionalSegmenterParameterDialog;
class ChtExtractorConfigDialog;

class BudExtractorTestWidget : public QWidget
{
  Q_OBJECT
  QVBoxLayout* mainLayout;
  ChtExtractorConfigDialog* chtDialog;
  //mode selection
  QGridLayout* modeLayout;
  QButtonGroup* modeGroup;
  QMap<QRadioButton*,ExtractorConfigDialog*> radioExtractorMap;
  QMap<QPushButton*,ExtractorConfigDialog*> configExtractorMap;
  //data selection
  QFormLayout* dataLayout;
  QString* dataFile;
  FileDirSelectionWidget* outputDirField;
  QComboBox* segmentationBox;
  ConvolutionalSegmenterParameterDialog* convolutionalSegmenterParameterDialog;
  //FileDirSelectionWidget* mapSelection;
  QDoubleSpinBox* positionToleranceBox;
  QPushButton* testButton;
  //testing
  BudExtractorTest budExtractorTest;
  //functions
  void addExtractor(ExtractorConfigDialog* dialog);
  BudExtractor* getCurrentBudExtractor();
  void saveSettings();
  void loadSettings();
public:
  explicit BudExtractorTestWidget(ChtExtractorConfigDialog* chtDialog, ConvolutionalSegmenterParameterDialog* convSegDialog, QString* dataFile, QWidget *parent = nullptr);

signals:

private slots:
  void configButtonPressed();
  void testButtonPressed();
};

#endif // BUDEXTRACTORTESTWIDGET_H

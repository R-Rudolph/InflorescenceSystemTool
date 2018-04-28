#ifndef CHTEXTRACTORCONFIGDIALOG_H
#define CHTEXTRACTORCONFIGDIALOG_H

#include "extractorconfigdialog.h"
#include "chtbudextractor.h"

class QSpinBox;
class QFormLayout;
class QLineEdit;
class QDoubleSpinBox;

class ChtExtractorConfigDialog : public ExtractorConfigDialog
{
  static const QString extractorName;
  ChtBudExtractor extractor;
  //
  QFormLayout* layout;
  QSpinBox* gaussianRadiusLcnInputBox;
  QSpinBox* gaussianRadiusLcnBox;
  QSpinBox* gaussianRadiusLcnOutputBox;
  QSpinBox* cannyThresholdLowerBox;
  QDoubleSpinBox* chtThresholdBox;
  QDoubleSpinBox* adjacentBlockingFactorBox;
  //
  virtual bool extractorToFields();
  virtual bool fieldsToExtractor();
  virtual void saveToSettings();
  virtual void loadFromSettings();
public:
  ChtExtractorConfigDialog(QWidget* parent, Qt::WindowFlags f = Qt::WindowFlags());
  ChtExtractorConfigDialog(const QString& settingsPrefix, QWidget* parent, Qt::WindowFlags f = Qt::WindowFlags());
  virtual const QString& name();
  virtual BudExtractor& getExtractor();
};

#endif // CHTEXTRACTORCONFIGDIALOG_H

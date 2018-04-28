#ifndef FRSTEXTRACTORCONFIGDIALOG_H
#define FRSTEXTRACTORCONFIGDIALOG_H

#include <QObject>
#include <QDialog>
#include "extractorconfigdialog.h"
#include "frstbudextractor.h"

class QFormLayout;
class QSpinBox;
class QDoubleSpinBox;
class QLineEdit;

class FrstExtractorConfigDialog : public ExtractorConfigDialog
{
  Q_OBJECT
  static const QString extractorName;
  FrstBudExtractor extractor;
  //fields
  QFormLayout* layout;
  QSpinBox* gaussianPreRadiusField;
  QDoubleSpinBox* gaussianPostStdDevField;
  QDoubleSpinBox* radialStrictnessField;
  QDoubleSpinBox* gradientThresholdField;
  QSpinBox* rangeMinField;
  QSpinBox* rangeMaxField;
protected:
  virtual bool extractorToFields();
  virtual bool fieldsToExtractor();
  virtual void saveToSettings();
  virtual void loadFromSettings();
public:
  explicit FrstExtractorConfigDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
  virtual const QString& name();
  virtual BudExtractor& getExtractor();

signals:

public slots:
};

#endif // FRSTEXTRACTORCONFIGDIALOG_H

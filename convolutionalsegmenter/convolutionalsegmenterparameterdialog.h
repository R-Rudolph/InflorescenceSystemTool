#ifndef CONVOLUTIONALSEGMENTERPARAMETERDIALOG_H
#define CONVOLUTIONALSEGMENTERPARAMETERDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include "convolutionalsegmenterparameters.h"

class QPushButton;
class QVBoxLayout;
class QFormLayout;
class QSpinBox;
class FileDirSelectionWidget;

class ConvolutionalSegmenterParameterDialog : public QDialog
{
  Q_OBJECT
  ConvolutionalSegmenterParameters previousParameters;
  QString settingsPrefix;

  QFormLayout* layout;
  QVBoxLayout* buttonLayout;
  QSpinBox* cropSizeField;
  FileDirSelectionWidget* prototxtField;
  FileDirSelectionWidget* caffemodelField;
  QPushButton* okButton;
  QPushButton* cancelButton;

  void saveSettings();
  void loadSettings();
public:
  explicit ConvolutionalSegmenterParameterDialog(QWidget *parent = nullptr, Qt::WindowFlags f=0);
  explicit ConvolutionalSegmenterParameterDialog(const QString& settingsPrefix, QWidget *parent = nullptr, Qt::WindowFlags f=0);

  ConvolutionalSegmenterParameters getParameters();
  void setParameters(const ConvolutionalSegmenterParameters& parameters);

  QString getSettingsPrefix() const;
  void setSettingsPrefix(const QString &value);

signals:

private slots:
  void ok();
  void cancel();
};

#endif // CONVOLUTIONALSEGMENTERPARAMETERDIALOG_H

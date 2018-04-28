#ifndef BUDEXTRACTORWIDGET_H
#define BUDEXTRACTORWIDGET_H

#include <QWidget>
#include <opencv2/core.hpp>

class FileDirSelectionWidget;
class QPushButton;
class QFormLayout;
class QDir;
class ChtExtractorConfigDialog;
class ConvolutionalSegmenterParameterDialog;
class ConvolutionalSegmenter;
class QTextStream;
class QComboBox;
class QCheckBox;

class BudExtractorWidget : public QWidget
{
  Q_OBJECT
  enum OutputMode
  {
    OUTPUT_SEGMENTATION,
    OUTPUT_HEATMAP,
    OUTPUT_BRIGHTEN
  };
  ChtExtractorConfigDialog* extractorDialog;
  ConvolutionalSegmenterParameterDialog* segmenterDialog;

  QFormLayout* layout;
  QComboBox* outputModeField;
  FileDirSelectionWidget* outputDirField;
  QCheckBox* saveSegmentationBox;
  QCheckBox* saveBudsBox;
  QPushButton* startButton;

  cv::Scalar getCircleColor();
  bool processImage(const QString& filepath, const QDir& outputDir, ConvolutionalSegmenter& segmenter, QTextStream& outputFile, QTextStream& outputFileInflorescences);
  void processImages(const QStringList& fileList, const QDir& outputDir);
public:
  explicit BudExtractorWidget(ChtExtractorConfigDialog* extractorDialog, ConvolutionalSegmenterParameterDialog* segmenterDialog, QWidget *parent = nullptr);

signals:

private slots:
  void start();
};

#endif // BUDEXTRACTORWIDGET_H

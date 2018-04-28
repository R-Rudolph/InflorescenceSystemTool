#ifndef INFLORESCENCESYSTEMWINDOW_H
#define INFLORESCENCESYSTEMWINDOW_H

#include <QMainWindow>

class QTabWidget;
class BudExtractorTestWidget;
class ParameterTesterWidget;
class SegmentationWidget;
class QMenu;
class QAction;
class SegmentationRuntimeTester;
class BudExtractorRuntimeTestWidget;
class SegmentationTesterWidget;
class ConvolutionalSegmenterParameterDialog;
class ChtExtractorConfigDialog;
class BudExtractorWidget;

class InflorescenceSystemWindow : public QMainWindow
{
  Q_OBJECT
  ConvolutionalSegmenterParameterDialog* convSegDialog;
  ChtExtractorConfigDialog* extractorDialog;

  QTabWidget* tabs;
  QTabWidget* runtimeTabs;
  QTabWidget* performanceTabs;
  BudExtractorWidget* extraction;
  SegmentationTesterWidget* segmentationTest;
  BudExtractorTestWidget* budExtractorTest;
  ParameterTesterWidget* parameterTest;
  SegmentationRuntimeTester* segmentationRuntimeTester;
  BudExtractorRuntimeTestWidget* budExtractorRuntimeTestWidget;
  //menu bar
  QString dataFile;
  QMenu* fileMenu;
  QMenu* configMenu;

  void updateTitle();
  void loadSettings();
  void saveSettings();
public:
  InflorescenceSystemWindow(QWidget *parent = 0);
  ~InflorescenceSystemWindow();
  QString getDataFile() const;
  void setDataFile(const QString &value);

private slots:
  void selectDataFile();
};

#endif // INFLORESCENCESYSTEMWINDOW_H

#ifndef PARAMETERTESTERWIDGET_H
#define PARAMETERTESTERWIDGET_H

#include <QWidget>
#include <queue>
#include "extractortest/budextractortest.h"

class QFormLayout;
class QPushButton;
class FileDirSelectionWidget;
class ChtExtractorConfigDialog;

class ParameterTesterWidget : public QWidget
{
  Q_OBJECT
  class ParameterCandidate
  {
    int cannyThreshold;
    float houghThreshold;
    float estimatedScore;
  public:
    ParameterCandidate();
    ParameterCandidate(int cannyThreshold, float houghThreshold, float estimatedScore);
    bool operator< (const ParameterCandidate& other) const;
    bool operator == (const ParameterCandidate& other) const;
    std::vector<ParameterCandidate> generateNewCandidates() const;
    int getCannyThreshold() const;
    void setCannyThreshold(int value);
    float getHoughThreshold() const;
    void setHoughThreshold(float value);
    float getEstimatedScore() const;
    void setEstimatedScore(float value);
  };
  QString* dataFile;
  BudExtractorTest tester;
  QFormLayout* dataLayout;
  QPushButton* configButton;
  QPushButton* startButton;
  //FileDirSelectionWidget* dataSelection;
  FileDirSelectionWidget* outputFileField;
  ChtExtractorConfigDialog* extractorDialog;
  void  loadSettings();
  void  saveSettings();
  void addNewCandidates(std::priority_queue<ParameterCandidate,std::vector<ParameterCandidate>>& queue, const std::vector<ParameterCandidate>& knownCandidates, const ParameterCandidate candidate);
  std::vector<ParameterCandidate> loadCandidates(QFile& file);
public:
  explicit ParameterTesterWidget(QString* dataFile, QWidget *parent = nullptr);
signals:

private slots:
  void startClicked();
  void configClicked();
};

#endif // PARAMETERTESTERWIDGET_H

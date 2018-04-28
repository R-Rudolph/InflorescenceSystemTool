#ifndef IMAGELIBRARY_H
#define IMAGELIBRARY_H

#include <QList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QStringList>
#include "imageinfo.h"

class ImageLibrary
{
  QList<ImageInfo> images;
public:
  ImageLibrary();
  bool load(const QString& libraryFilepath);
  bool save(const QString& libraryFilepath);
  void addImage(const QString& filename);
  void addImages(const QStringList& filenames);
  ImageInfo* infoByFilename(const QString& filename);
  QList<ImageInfo>& getInfos();
  int getInfoCount();
};

#endif // IMAGELIBRARY_H

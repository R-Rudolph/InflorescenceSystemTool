#include "imagelibrary.h"

ImageInfo *ImageLibrary::infoByFilename(const QString &filename)
{
  for(int i=0;i<images.size();i++)
  {
    if(images[i].getFilepath()==filename)
      return &(images[i]);
  }
  return nullptr;
}

QList<ImageInfo> &ImageLibrary::getInfos()
{
  return  images;
}

int ImageLibrary::getInfoCount()
{
  return images.size();
}

ImageLibrary::ImageLibrary()
{

}

bool ImageLibrary::load(const QString &libraryFilepath)
{
  QFile file(libraryFilepath);
  if(file.open(QIODevice::ReadOnly))
  {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(),&error);
    file.close();
    if(error.error != QJsonParseError::NoError)
      return false;
    QJsonArray fileArray = doc.array();
    for(int i=0;i<fileArray.size();i++)
    {
      images.append(ImageInfo(fileArray[i].toObject()));
    }
    return true;
  }
  else
  {
    return false;
  }
}

bool ImageLibrary::save(const QString &libraryFilepath)
{
  QFile file(libraryFilepath);
  if(file.open(QIODevice::WriteOnly))
  {
    QJsonArray fileArray;
    for(int i=0;i<images.size();i++)
    {
      fileArray.append(images[i].toJson());
    }
    QJsonDocument doc(fileArray);
    file.write(doc.toJson());
    file.close();
    return true;
  }
  else
    return false;
}

void ImageLibrary::addImage(const QString &filename)
{
  if(infoByFilename(filename)==nullptr)
    images.append(ImageInfo(filename));
}

void ImageLibrary::addImages(const QStringList &filenames)
{
  for(int i=0;i<filenames.size();i++)
  {
    addImage(filenames[i]);
  }
}


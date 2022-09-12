#ifndef IRENDERCOREFACTORY_H
#define IRENDERCOREFACTORY_H
#include "irendercore.h"
#include "rendercorebasic.h"
#include "rendercoreblue.h"
#include <QMap>
#include <QString>
/*
 * The factory to manage the render cores.
 * The Factory will new the object on the leap,
 * so remember to delete it.
 */
enum RenderCoreID { Basic, Blue };

class IRenderCoreFactory {
private:
  QMap<QString, RenderCoreID> renderMap;

public:
  IRenderCoreFactory() {
    renderMap.insert("Basic", Basic);
    renderMap.insert("Blue", Blue);
  }

public:
  QMap<QString, RenderCoreID> getMap() { return this->renderMap; }
  IRenderCore *getRenderCore(QString renderCoreName) {
    if (!renderMap.contains(renderCoreName)) {
      return nullptr;
    }
    switch (renderMap[renderCoreName]) {
    case Basic:
      return new RenderCores::RenderCoreBasic;
    case Blue:
      return new RenderCores::RenderCoreBlue;
    default:
      return nullptr;
    }
  }
};

#endif // IRENDERCOREFACTORY_H

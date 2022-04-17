#include "OCCView.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>

int main(int argc, char *argv[]) {

  QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

  QGuiApplication app(argc, argv);
  QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

  qmlRegisterType<OCCT>("OpenCascade", 7, 6, "OCCT");
  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  return app.exec();
}

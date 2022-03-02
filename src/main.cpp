#include "OCCView.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>

int main(int argc, char *argv[]) {
  //    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGLRhi);

  //#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  //    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  //#endif

  QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
  //  OSD_Environment aVarRendLoop("QSG_RENDER_LOOP");
  //  if (aVarRendLoop.Value().IsEmpty()) {
  //    aVarRendLoop.SetValue("basic");
  //    aVarRendLoop.Build();
  //  }
  //   // QCoreApplication::setAttribute (Qt::AA_UseDesktopOpenGL);
  QGuiApplication app(argc, argv);
  QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

  qmlRegisterType<OCCT>("OpenCascade", 7, 6, "OCCT");
  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  //    QQmlApplicationEngine engine;
  //    const QUrl url(QStringLiteral("qrc:/main.qml"));
  //    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
  //                     &app, [url](QObject *obj, const QUrl &objUrl) {
  //        if (!obj && url == objUrl)
  //            QCoreApplication::exit(-1);
  //    }, Qt::QueuedConnection);
  //    engine.load(url);

  //    return app.exec();
  //    QGuiApplication app(argc, argv);

  //    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
  //   // qmlRegisterType<OCCT> ("OpenCascade", 7, 6, "OCCView");
  //    QQuickView view;
  //    view.setResizeMode(QQuickView::SizeRootObjectToView);
  //    view.setSource(QUrl("qrc:/main.qml"));
  //    view.show();

  return app.exec();
}

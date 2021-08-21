#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "WordsModelController.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QApplication app(argc, argv);

	QQmlApplicationEngine engine;
	auto * qmlContext = engine.rootContext();
	qmlContext->setContextProperty("wordsModelController", new WordsModelController());

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty())
		return EXIT_FAILURE;

	return QApplication::exec();
}

#pragma once
#include <QObject>

class QFile;

class WordsModelController final : public QObject
{
	Q_OBJECT

public:
	explicit WordsModelController(QObject * parent = nullptr);

	Q_INVOKABLE [[nodiscard]] QVariantList GetWords() const;
	Q_INVOKABLE [[nodiscard]] QVariantList GetValues() const;
	Q_INVOKABLE [[nodiscard]] int GetYAxisMaxValue() const;
	Q_INVOKABLE void ProcessFile(const QUrl & file);

signals:
	void fileBeingProcessed();
	void fileProcessed();

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};

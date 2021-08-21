#pragma once

#include "WordsModel.h"

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
	WordsModel m_model;
};

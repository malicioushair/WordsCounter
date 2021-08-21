#pragma once

#include <QAbstractListModel>

class QFile;

class WordsModel final : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit WordsModel(QObject * parent = nullptr);
	~WordsModel();

	WordsModel(const WordsModel & other) = delete;
	WordsModel(const WordsModel && other) = delete;
	WordsModel & operator=(const WordsModel & other) = delete;
	WordsModel & operator=(const WordsModel && other) = delete;

	[[nodiscard]] int rowCount(const QModelIndex & parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex & index, int role) const override;
	bool setData(const QModelIndex & index, const QVariant & value, int role) override;

signals:
	void DataReady();

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};

#pragma once

#include <QAbstractListModel>
#include "NonCopyMovable.h"

class QFile;

class WordsModel final : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit WordsModel(QObject * parent = nullptr);
	~WordsModel();

	NON_COPY_MOVABLE(WordsModel)

	[[nodiscard]] int rowCount(const QModelIndex & parent) const override;
	[[nodiscard]] QVariant data(const QModelIndex & index, int role) const override;
	bool setData(const QModelIndex & index, const QVariant & value, int role) override;

signals:
	void DataReady();

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};

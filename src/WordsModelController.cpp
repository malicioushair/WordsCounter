#include "WordsModelController.h"

#include <QUrl>

#include "WordsModel.h"
#include "WordsModelRole.h"

struct WordsModelController::Impl
{
	WordsModel model;
};

WordsModelController::WordsModelController(QObject *parent)
	: QObject(parent)
	, m_impl(std::make_unique<Impl>())
{
	connect(&m_impl->model, &WordsModel::DataReady, [&] () {
		emit fileProcessed();
	});
}

QVariantList WordsModelController::GetWords() const
{
	const auto words = m_impl->model.data({}, AllTexts);
	return words.toList();
}

QVariantList WordsModelController::GetValues() const
{
	const auto values = m_impl->model.data({}, AllValues);
	return values.toList();
}

int WordsModelController::GetYAxisMaxValue() const
{
	const auto values = m_impl->model.data({}, AllValues);
	const auto valuesList = values.toList();
	const auto max = std::max_element(valuesList.cbegin(), valuesList.cend());
	assert(max != nullptr);

	return max->toInt();
}

void WordsModelController::ProcessFile(const QUrl & fileUrl)
{
	emit fileBeingProcessed();
	m_impl->model.setData({}, QVariant::fromValue(fileUrl), ReFillData);
}

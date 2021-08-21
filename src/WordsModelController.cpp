#include "WordsModelController.h"

#include <QUrl>

#include "WordsModelRole.h"


WordsModelController::WordsModelController(QObject *parent)
	: QObject(parent)
{
	connect(&m_model, &WordsModel::DataReady, [&] () {
		emit fileProcessed();
	});
}

QVariantList WordsModelController::GetWords() const
{
	const auto text = m_model.data({}, AllTexts);
	return text.toList();
}

QVariantList WordsModelController::GetValues() const
{
	const auto values = m_model.data({}, AllValues);
	return values.toList();
}

int WordsModelController::GetYAxisMaxValue() const
{
	const auto values = m_model.data({}, AllValues);
	const auto valuesList = values.toList();
	const auto max = std::max_element(valuesList.cbegin(), valuesList.cend());
	assert(max != nullptr);

	return max->toInt();
}

void WordsModelController::ProcessFile(const QUrl & fileUrl)
{
	emit fileBeingProcessed();
	m_model.setData({}, QVariant::fromValue(fileUrl), ReFillData);
}

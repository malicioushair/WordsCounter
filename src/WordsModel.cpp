#include "WordsModel.h"

#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>

#include "WordsModelRole.h"

using WordsModelItems = std::vector<std::pair<int, QString>>;
using WordsFrequencyMap = std::map<QString, int>;

namespace {

constexpr int MAX_NUMBER_OF_ITEMS_TO_DISPLAY = 15;

enum class ModelItem
{
	Key,
	Value,
};

std::multimap<int, QString, std::greater<>> Invert(const WordsFrequencyMap & mapToInvert)
{
	std::multimap<int, QString, std::greater<>> multiMap;
	std::transform(mapToInvert.cbegin(), mapToInvert.cend(), std::inserter(multiMap, multiMap.end()), [](const std::pair<QString, int> & item) {
		return std::make_pair(item.second, item.first);
	});

	return multiMap;
}

WordsFrequencyMap ReadFile(const QUrl & fileUrl)
{
	QFile file(fileUrl.toLocalFile());

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return {};

	WordsFrequencyMap wordsMap;
	while (!file.atEnd()) {
		QRegExp separator("[,.!?\\-\\s]");
		const auto line = QString(file.readLine());
		const auto words = line.split(separator);
		for (const auto & word : words)
			if (!word.isEmpty())
				++wordsMap[word.toLower()];
	}

	return wordsMap;
}

QVariantList GetFromData(const WordsModelItems & data, const ModelItem pairItem)
{
	QVariantList result;
	for (const auto & item : data)
		if (pairItem == ModelItem::Key)
			result.push_back(item.first);
		else
			result.push_back(item.second);

	return result;
}

}

struct WordsModel::Impl
{
	WordsModelItems data;
	QFutureWatcher<WordsFrequencyMap> watcher;
	QFuture<WordsFrequencyMap> future;
};

WordsModel::WordsModel(QObject * parent)
	: QAbstractListModel(parent)
	, m_impl(std::make_unique<Impl>())
{
	connect(&m_impl->watcher, &QFutureWatcher<WordsFrequencyMap>::finished, [&]() {
		const auto wordsMap = m_impl->future.result();
		const auto invertedMap = Invert(wordsMap);

		std::copy_n(invertedMap.cbegin(), MAX_NUMBER_OF_ITEMS_TO_DISPLAY, std::inserter(m_impl->data, m_impl->data.end()));
		std::sort(m_impl->data.rbegin(), m_impl->data.rend(), [](const auto & lhs, const auto & rhs) { return lhs.second > rhs.second; });

		emit DataReady();
	});
}

WordsModel::~WordsModel() = default;

int WordsModel::rowCount(const QModelIndex &) const
{
	return m_impl->data.size();
}

QVariant WordsModel::data(const QModelIndex & index, const int role) const
{
	switch (role)
	{
		case AllTexts:
			return GetFromData(m_impl->data, ModelItem::Value);
		case AllValues:
			return GetFromData(m_impl->data, ModelItem::Key);
	}

	if (!index.isValid())
		return {};

	const auto [text, frequency] = m_impl->data.at(static_cast<size_t>(index.row()));

	switch (role)
	{
		case Value:
			return frequency;
		case Text:
			return text;
		default:
			assert(false && "Unexpected role!");
			break;
	}

	return {};
}

bool WordsModel::setData(const QModelIndex &, const QVariant & value, const int role)
{
	switch (role)
	{
		case ReFillData:
		{
			m_impl->data.clear();

			const auto fileUrl = value.toUrl();
			m_impl->future = QtConcurrent::run(ReadFile, fileUrl);
			m_impl->watcher.setFuture(m_impl->future);

			return true;
		}
		default:
			assert(false && "Unexpected role!");
			break;
	}

	return false;
}

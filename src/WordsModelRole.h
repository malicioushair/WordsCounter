#pragma once

enum WordsModelRole
{
	// getter roles
	AllTexts = Qt::UserRole + 1,
	AllValues,
	Text,
	Value,

	// setter roles
	ReFillData,
};

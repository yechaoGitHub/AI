#pragma once

#include <QPushButton>

class WLabelButton  : public QPushButton
{
	Q_OBJECT

public:
	WLabelButton(QWidget *parent);
	~WLabelButton();

	void	setSelected(bool sel);
	void	setType(int type, const QString& typeName);
	int		getType(){
		return _type;
	}
private:
	int			_type = 0;
	QString		_typeName;
};

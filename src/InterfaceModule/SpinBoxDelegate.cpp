#include "SpinBoxDelegate.h"
#include "TreeItem.h"
#include <QPalette>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QDebug>
#include <QApplication>

SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
    : QItemDelegate(parent)
{
  _state=QStyle::State_Enabled;
}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex & index ) const
{
  if(index.data().type()==QVariant::Double){
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setMinimum(-9999999);
    editor->setMaximum(999999);
    editor->setValue(index.data().toDouble());
    return editor;
  }else{
    QLineEdit * editor = new QLineEdit(parent);
    editor->setText(index.data().toString());
    return editor;
  }
}

void SpinBoxDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  if(static_cast<TreeItem*>(index.internalPointer())->isButton()&&index.column()==1){
    const QStandardItemModel* model = static_cast<const QStandardItemModel*>(index.model());
    QString text=static_cast<TreeItem*>(index.internalPointer())->getData().toString();
    QRect rect = option.rect;

    QRect buttonRect( rect);
    QStyleOptionButton button;
    button.rect = buttonRect;
    button.text = text;
    button.state = _state | QStyle::State_Enabled;

    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
  }else{
    QItemDelegate::paint(painter, option, index);
  }
}

void SpinBoxDelegate::setEditorData(QWidget *editor,
				    const QModelIndex &index) const
{

  if(index.data().type()==QVariant::Double){
    int value = index.model()->data(index, Qt::EditRole).toDouble();
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
  }else{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *lineEdit=static_cast<QLineEdit*>(editor);
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
  }
}

  void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
				     const QModelIndex &index) const
{
  TreeItem* item=static_cast<TreeItem*>(index.internalPointer());
  if(index.data().type()==QVariant::Double){
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
  }else{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

    model->setData(index, lineEdit->text(), Qt::EditRole);
  }
  emit item->valueChanged();
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}

bool SpinBoxDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
  if(static_cast<TreeItem*>(index.internalPointer())->isButton()&&index.column()==1){
    if( event->type() == QEvent::MouseButtonPress ||
	event->type() == QEvent::MouseButtonRelease ) {
    } else {
	  //ignoring other mouse event and reseting button's state
	  _state = QStyle::State_Raised;
	return true;
    }

    QRect buttonRect( option.rect);

    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    if( !buttonRect.contains( mouseEvent->pos()) ) {
	_state = QStyle::State_Raised;
	return true;
    }

    if( event->type() == QEvent::MouseButtonPress) {            
	_state = QStyle::State_Sunken;
    } else if( event->type() == QEvent::MouseButtonRelease) {
	_state = QStyle::State_Raised;
	static_cast<TreeItem*>(index.internalPointer())->getCallBack()();
    }    
    return true;
  }else{
    return QItemDelegate::editorEvent(event, model, option, index);
  }
}

QSize SpinBoxDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QItemDelegate::sizeHint(option, index);
}


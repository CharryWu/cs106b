/*
 * File: gcheckbox.cpp
 * -------------------
 *
 * @author Marty Stepp
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gcheckbox.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#include "gcheckbox.h"
#include "gthread.h"
#include "gwindow.h"
#include "strlib.h"

GCheckBox::GCheckBox(const std::string& text, bool checked, QWidget* parent) {
    GThread::runOnQtGuiThread([this, checked, parent]() {
        _iqcheckBox = new _Internal_QCheckBox(this, checked, getInternalParent(parent));
    });
    setText(text);
    setVisible(false);   // all widgets are not shown until added to a window
}

GCheckBox::~GCheckBox() {
    // TODO: delete _iqcheckBox;
    _iqcheckBox = nullptr;
}

std::string GCheckBox::getActionCommand() const {
    if (_actionCommand.empty()) {
        return getText();
    } else {
        return _actionCommand;
    }
}

_Internal_QWidget* GCheckBox::getInternalWidget() const {
    return _iqcheckBox;
}

std::string GCheckBox::getText() const {
    return _iqcheckBox->text().toStdString();
}

std::string GCheckBox::getType() const {
    return "GCheckBox";
}

QWidget* GCheckBox::getWidget() const {
    return static_cast<QWidget*>(_iqcheckBox);
}

bool GCheckBox::isChecked() const {
    return _iqcheckBox->isChecked();
}

bool GCheckBox::isSelected() const {
    return _iqcheckBox->isChecked();
}

void GCheckBox::removeActionListener() {
    removeEventListener("change");
}

void GCheckBox::removeDoubleClickListener() {
    removeEventListener("doubleclick");
}

void GCheckBox::setActionListener(GEventListener func) {
    setEventListener("change", func);
}

void GCheckBox::setActionListener(GEventListenerVoid func) {
    setEventListener("change", func);
}

void GCheckBox::setDoubleClickListener(GEventListener func) {
    setEventListener("doubleclick", func);
}

void GCheckBox::setDoubleClickListener(GEventListenerVoid func) {
    setEventListener("doubleclick", func);
}

void GCheckBox::setChecked(bool checked) {
    GThread::runOnQtGuiThread([this, checked]() {
        _iqcheckBox->setChecked(checked);
    });
}

void GCheckBox::setSelected(bool selected) {
    setChecked(selected);
}

void GCheckBox::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqcheckBox->setText(QString::fromStdString(text));
    });
}


_Internal_QCheckBox::_Internal_QCheckBox(GCheckBox* gcheckBox, bool checked, QWidget* parent)
        : QCheckBox(parent),
          _gcheckBox(gcheckBox) {
    setChecked(checked);
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(handleStateChange(int)));
}

void _Internal_QCheckBox::handleStateChange(int /* state */) {
    GEvent changeEvent(
                /* class  */ CHANGE_EVENT,
                /* type   */ STATE_CHANGED,
                /* name   */ "change",
                /* source */ _gcheckBox);
    changeEvent.setActionCommand(_gcheckBox->getActionCommand());
    _gcheckBox->fireEvent(changeEvent);
}

void _Internal_QCheckBox::mouseDoubleClickEvent(QMouseEvent* event) {
    QWidget::mouseDoubleClickEvent(event);   // call super
    emit doubleClicked();
    if (!_gcheckBox->isAcceptingEvent("doubleclick")) return;
    GEvent mouseEvent(
                /* class  */ MOUSE_EVENT,
                /* type   */ MOUSE_DOUBLE_CLICKED,
                /* name   */ "doubleclick",
                /* source */ _gcheckBox);
    mouseEvent.setActionCommand(_gcheckBox->getActionCommand());
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->x());
    mouseEvent.setY(event->y());
    _gcheckBox->fireEvent(mouseEvent);
}

QSize _Internal_QCheckBox::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QCheckBox::sizeHint();
    }
}

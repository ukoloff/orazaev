#include "widget_manager.h"

#include <iostream>

int main() {
    NewWidgetManager newWm;
    NewClonWidgetManager newClonWm;
    ProtoClonWidgetManager protoClonWm;

    Widget* anWidget = newWm.create();
    std::cout << anWidget->id() << std::endl; // 0
    delete anWidget;

    ClonableWidget* theWidget = newClonWm.create();
    theWidget->id() = 42;
    std::cout << theWidget->id() << std::endl; // 42
    protoClonWm.switchPrototype(theWidget);

    ClonableWidget* someWidget = protoClonWm.create();

    theWidget->id() = 13;
    std::cout << theWidget->id() << std::endl; // 13
    std::cout << someWidget->id() << std::endl; // 42

    delete someWidget;
    delete theWidget;

    return 0;
}

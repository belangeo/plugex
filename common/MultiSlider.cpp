/*******************************************************************************
* Plugex - PLUGin EXamples
*
* Plugex est une série de plugiciels auto-documentés permettant une étude 
* autonome du développement de plugiciels avec JUCE ainsi que des bases du
* traitement de signal audio avec le langage C++.
*
* © Olivier Bélanger 2019
*
*******************************************************************************/

#include "MultiSlider.h"

MultiSlider::MultiSlider() {}

MultiSlider::~MultiSlider() {
    setLookAndFeel(nullptr);
}

void MultiSlider::setup(int numberOfPoints) {
    lastPosition.setXY(0, 0);
    bars.resize(numberOfPoints);
}

void MultiSlider::paint(Graphics& g) {
    int width = getWidth();
    int height = getHeight();

    PlugexLookAndFeel *lookAndFeel;
    lookAndFeel = static_cast<PlugexLookAndFeel *> (&getLookAndFeel());

    g.fillAll (lookAndFeel->findColour (ResizableWindow::backgroundColourId));

    g.setColour(lookAndFeel->getLightTheme());
    g.drawRect(0, 0, width, height);

    for (int i = 0; i < width; i++) {
        g.drawVerticalLine(i, height - height * bars[i], height);
    }
}

void MultiSlider::resized() {
    // Maybe we should use bars.resize() for the case where the size of the multislider change.
    // But that implies that the processor is aware of this change in size...
}

void MultiSlider::setPoints(const Array<float> &points) {
    for (int i = 0; i < points.size(); i++) {
        bars.set(i, points[i]);
    }

    repaint();

    listeners.call([&] (Listener& l) { l.multiSliderChanged(bars); });
}

void MultiSlider::mouseDown(const MouseEvent &event) {
    int height = getHeight();

    lastPosition.setXY(event.x, event.y);

    bars.set(lastPosition.x, (height - event.y) / (float)height);

    listeners.call([&] (Listener& l) { l.multiSliderChanged(bars); });

    repaint();
}

void MultiSlider::mouseDrag(const MouseEvent &event) {    
    int width = getWidth();
    int height = getHeight();

    int lastX = lastPosition.x < 0 ? 0 : lastPosition.x >= width ? width - 1 : lastPosition.x;
    int lastY = lastPosition.y < 0 ? 0 : lastPosition.y >= height ? height - 1 : lastPosition.y;

    int newX = event.x < 0 ? 0 : event.x >= width ? width - 1 : event.x;
    int newY = event.y < 0 ? 0 : event.y >= height ? height - 1 : event.y;

    float y1 = (float)lastY;
    float y2 = (float)newY;
    float diff = y2 - y1;
    int steps = jmax(lastX, newX) - jmin(lastX, newX);
    int direction = lastX < newX ? 1 : -1;

    for (int i = 0; i < steps; i++) {
        bars.set(lastX+i*direction, (height - (y1 + diff)) / (float)height);
    } 

    lastPosition.setXY(newX, newY);

    listeners.call([&] (Listener& l) { l.multiSliderChanged(bars); });

    repaint();
}

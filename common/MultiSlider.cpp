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

MultiSlider::MultiSlider() {
    lastPosition.setXY(0, 0);
    initialized = false;
}

MultiSlider::~MultiSlider() {
    setLookAndFeel(nullptr);
}

void MultiSlider::paint(Graphics& g) {
    int width = getWidth();
    int height = getHeight();

    if (! initialized) {
        initialized = true;
        for (int i = 0; i < width; i++) {
            bars.push_back(0.0f);
        }
    }

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
    bars.clear();
    initialized = false;
}

void MultiSlider::mouseDown(const MouseEvent &event) {
    int height = getHeight();

    lastPosition.setXY(event.x, event.y);

    bars[lastPosition.x] = (height - event.y) / (float)height;

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
        bars[lastX+i*direction] = (height - (y1 + diff)) / (float)height;
    } 

    lastPosition.setXY(newX, newY);

    listeners.call([&] (Listener& l) { l.multiSliderChanged(bars); });

    repaint();
}

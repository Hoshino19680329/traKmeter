/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2013 Martin Zuther (http://www.mzuther.de/)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Thank you for using free software!

---------------------------------------------------------------------------- */

#ifndef __METER_BAR_AVERAGE_H__
#define __METER_BAR_AVERAGE_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "meter_segment.h"


//==============================================================================
/**
*/
class MeterBarAverage : public Component
{
public:
    MeterBarAverage(const String& componentName, int pos_x, int pos_y, int width, int number_of_bars, int crest_factor, int segment_height, bool display_peaks);
    ~MeterBarAverage();

    void setLevels(float averageLevel, float averageLevelPeak);
    void paint(Graphics& g);
    void resized();
    void visibilityChanged();

private:
    JUCE_LEAK_DETECTOR(MeterBarAverage);

    float fAverageLevel;
    float fAverageLevelPeak;

    int nPosX;
    int nPosY;
    int nWidth;
    int nHeight;
    int nSegmentHeight;
    int nNumberOfBars;

    MeterSegment** pMeterSegments;
};


#endif  // __METER_BAR_AVERAGE_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:

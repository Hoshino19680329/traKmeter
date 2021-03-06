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

#include "average_meter.h"

AverageMeter::AverageMeter(const String& componentName, int posX, int posY, int width, int CrestFactor, int nNumChannels, int segment_height)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;
    nCrestFactor = CrestFactor;

    nNumberOfBars = 8;
    nSegmentHeight = segment_height;
    nMeterPositionBottom = 21;
    nMeterHeight = nNumberOfBars * nSegmentHeight + 1;

    nPosX = posX;
    nPosY = posY;
    nWidth = width;
    nHeight = nMeterHeight + nMeterPositionBottom;

    int nPositionX = 0;
    LevelMeters = new MeterBarAverage*[nInputChannels];

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;

        LevelMeters[nChannel] = new MeterBarAverage("Level Meter Average #" + String(nChannel), nPositionX, 0, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nNumberOfBars, nCrestFactor, nSegmentHeight, true);
        addAndMakeVisible(LevelMeters[nChannel]);
    }
}


AverageMeter::~AverageMeter()
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        delete LevelMeters[nChannel];
        LevelMeters[nChannel] = NULL;
    }

    delete [] LevelMeters;
    LevelMeters = NULL;

    deleteAllChildren();
}


int AverageMeter::getPreferredHeight()
{
    return nHeight;
}


void AverageMeter::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);
}


void AverageMeter::paint(Graphics& g)
{
    int x = 0;
    int y = nMeterHeight + nMeterPositionBottom - 13;
    int width = TraKmeter::TRAKMETER_LABEL_WIDTH - 14;
    int height = 13;

    g.setColour(Colours::grey.brighter(0.6f));
    g.fillRect(x, y, width + 3, height);

    g.setColour(Colours::black);
    g.drawRect(x, y, width + 3, height);

    int x_2 = TraKmeter::TRAKMETER_LABEL_WIDTH + nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 1;
    g.setColour(Colours::grey.brighter(0.6f));
    g.fillRect(x + x_2, y, width + 3, height);

    g.setColour(Colours::black);
    g.drawRect(x + x_2 + x, y, width + 3, height);

    String strMarker = "dB";

    g.setColour(Colours::black);
    g.setFont(12.0f);
    drawMarkers(g, strMarker, x + 1, y + 1, width, height);

    g.setColour(Colours::grey.brighter(0.6f));
    g.fillRect((nWidth - 32) / 2, y, 32, height);

    g.setColour(Colours::black);
    g.drawRect((nWidth - 32) / 2, y, 32, height);

    g.setColour(Colours::black);
    g.drawFittedText("RMS", (nWidth - 30) / 2, y, 30, height, Justification::centred, 1, 1.0f);

    y = 0;
    strMarker = "HOT";

    g.setFont(12.0f);
    g.setColour(Colours::red);
    drawMarkers(g, strMarker, x + 1, y, width, height);

    y -= TraKmeter::TRAKMETER_SEGMENT_HEIGHT / 2;

    for (int n = nNumberOfBars; n > 2; n -= 2)
    {
        int nLevel = nCrestFactor + n - 26;

        if (nLevel > 0)
        {
            strMarker = "+" + String(nLevel);
        }
        else
        {
            strMarker = String(nLevel);
        }

        y += 2 * nSegmentHeight;

        if (nLevel == nCrestFactor - 20)
        {
            g.setColour(Colours::white);
        }
        else
        {
            g.setColour(Colours::white.darker(0.4f));
        }

        drawMarkers(g, strMarker, x + 1, y, width, height);
    }

    y -= TraKmeter::TRAKMETER_SEGMENT_HEIGHT / 2;
    strMarker = "LOW";

    g.setColour(Colours::yellow);
    drawMarkers(g, strMarker, x + 1, y + 2 * nSegmentHeight, width, height);
}


void AverageMeter::resized()
{
}


void AverageMeter::setLevels(MeterBallistics* pMeterBallistics)
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        LevelMeters[nChannel]->setLevels(pMeterBallistics->getAverageMeterLevel(nChannel), pMeterBallistics->getAverageMeterPeakLevel(nChannel));
    }
}


void AverageMeter::drawMarkers(Graphics& g, String& strMarker, int x, int y, int width, int height)
{
    g.saveState();

    int meter_width = nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 4;
    int x_2 = x + TraKmeter::TRAKMETER_LABEL_WIDTH + meter_width + 4;

    g.drawFittedText(strMarker, x, y - 1, width, height, Justification::centred, 1, 1.0f);
    g.drawFittedText(strMarker, x_2, y - 1, width, height, Justification::centred, 1, 1.0f);

    g.setColour(Colours::grey);

    int nMarkerY = y + 5;
    int nStart = x + width + 2;
    int nEnd = x + x_2 - 3;

    for (int nMarkerX = nStart; nMarkerX < nEnd; nMarkerX++)
    {
        g.setPixel(nMarkerX, nMarkerY);
    }

    g.restoreState();
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:

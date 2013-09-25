# Pebble Counter

A simple counter app for the Pebble smartwatch using standard system components like ActionBarLayer.

## Versions

Various types of behaviour can be configoured. To change these, edit the preprocessor `#defines` in `src/counter.c`.

* `#define COUNTER_START 0`: Sets the start and reset value of the counter
* `#define COUNTER_MAX 9999`: The maximum value the counter may reach. It is recommended to use no more than 4 digits, otheriwse the number will overflow the TextLayer and be clipped.
* `#define COUNTER_MIN -9999`: The minimum value the counter may reach. It is recommended to use no more than 4 digits (excluding the `-`), otherwiese the number will overflow the TextLayer and be clipped.
* `#define HOLD_TO_REPEAT_MS 100`: The amount of milliseconds between two triggered clicks when holding the button.
* `#define MAX_DIGITS`: The naximum amout of digits that can be written to the display. Necessary because of the memory management drawbacks of the Pebble
* `//#define HOLD_JUMP 10`: If this value is defined, hold-to-repeat will be disabled and holding the buttons will jump the amount defined here.

The branch `mtg-life-points` contains a set of values tuned specifically for use as a life points counter in the Magic: the Gathering trading card game. It also uses a different UUID, icon and name. Please Wizards of the Cost/Hasbro don't sue me :)

## Precompiled `.pbw` files
You can download builds here:

* [Counter](http://upload.ps0ke.de/bin/pebble/counter/counter-1.0.pbw)

![Counter](http://api.qrserver.com/v1/create-qr-code/?data=http%3A%2F%2Fupload.ps0ke.de%2Fbin%2Fpebble%2Fcounter%2Fcounter-1.0.pbw&size=100x100)

* [MtG Life Points](http://upload.ps0ke.de/bin/pebble/counter/mtg_life_points-1.0.pbw)

![MtG Life Points](http://api.qrserver.com/v1/create-qr-code/?data=http%3A%2F%2Fupload.ps0ke.de%2Fbin%2Fpebble%2Fcounter%2Fmtg_life_points-1.0.pbw&size=100x100)

## License

    Copyright (c) 2013 Ps0ke

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

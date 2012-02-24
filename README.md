Audio CMOS Rate Monitor
=======================
Find light leaks by hearing the detector scream.

Example:

    $ python cmos_monitor.py tcp://localhost:5024 crate 1.0

Syntax:

    python cmos_monitor.py ADDRESS MODE PRESCALE

    ADDRESS (required)
        Address of dispatcher server (e.g. ORCA)

    MODE (optional, default: crate)
        What to do.
        Options:
            * `crate`: max frequency for each crate, 19 tones at once
            * `slot`: max frequency for each slot, 19*16 tones at once. probably crashes computer.
            * `channel`: frequency for each channel, 19*16*32 tones at once. disabled bc definitely crashes computer.
            * `max`: max frequency for whole detector, 1 tone

    PRESCALE (optional)
        Multiply the rates by this number to compute the tone that is played. Can be <1.

Testing
-------
To test the monitor, you can run `test_server` (build with `./build.sh`, have to `.L PackedEvent.hh++` first):

    ./test_server MODE

    MODE (required)
        * `noise`: random frequency on each channel
        * `song`: play a song on 0/0/0
        * `crates`: play 110 on crate 0, 220 on crate 1, etc. all on channel 5/0.
        * `tone`: play an A440 on 0/0/0.

Use `localhost:5024` as the ADDRESS for cmos_monitor.py.


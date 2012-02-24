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


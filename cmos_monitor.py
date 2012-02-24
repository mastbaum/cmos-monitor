#!/usr/bin/env python

import sys
import avalanche
import audiere
import numpy
import ROOT

ROOT.gROOT.ProcessLine(".L PackedEvent.hh++")

CRATES = 19
SLOTS = 16
CHANNELS = 32

def get_sc(n):
    slot = (n & 0x1e0) >> 5
    chan = n & 0x1f
    return slot, chan

class RateSpeaker:
    def __init__(self, freq, prescale=1):
        self.freq = freq
        self.prescale = prescale
    def start(self, device, freq):
        print 'start'
        self.freq = freq
        if hasattr(self, 'stream'):
            self.stop()
        self.stream = device.create_tone(self.freq / self.prescale)
        self.stream.play()
    def stop(self):
        if hasattr(self, 'stream'):
            print 'stop'
            self.stream.stop()

VRateSpeaker = numpy.vectorize(RateSpeaker)

class RateMonitor:
    def __init__(self, mode='max', prescale=1):
        devices = audiere.get_devices()
        if 'oss' in devices:
            self.device = audiere.open_device('oss')
        elif 'alsa' in devices:
            self.device = audiere.open_device('alsa')
        else:
            raise RuntimeError('no suitable audio device found!')

        self.mode = mode
        self.channels = numpy.zeros((CRATES, SLOTS, CHANNELS), dtype=numpy.float32)
        self.prescale = numpy.ones_like(self.channels) * prescale

        self.speakers = numpy.empty_like(self.channels, dtype=object)
        self.speakers[:,:,:] = VRateSpeaker(self.channels, prescale)

    def update_channel(self, crate, slot, channel, freq):
        print 'update channel:', crate, slot, channel, freq
        self.channels[crate][slot][channel] = freq / self.prescale[crate][slot][channel]

    def update_speaker(self):
        if self.mode == 'max':
            max_freq = numpy.max(self.channels)
            print 'start 0 0 0'
            self.speakers[0][0][0].start(self.device, max_freq)

        if self.mode == 'crate':
            print 'start %s 0 0' % crate
            max_freq = numpy.max(numpy.max(self.channels, axis=2), axis=1)
            self.speakers[crate][0][0].start(self.device, max_freq[crate])

        if self.mode == 'slot':
            print 'start %s % 0' % (crate, slot)
            max_freq = numpy.max(self.channels, axis=2)
            self.speakers[crate][slot][0].start(self.device, max_freq[crate][slot])

        if self.mode == 'channel':
            print '10000 streams probably not a good idea'
            #self.speakers[crate][slot][channel].start(self.device, frequency)

if __name__ == '__main__':
    client = avalanche.Client()
    client.add_dispatcher(sys.argv[1])

    mode = 'crate'
    if len(sys.argv) > 2:
        mode = sys.argv[2]
        if mode not in ['max', 'crate', 'slot', 'channel']:
            print 'unknown mode'
            sys.exit(1)

    prescale = 1.0
    if len(sys.argv) > 3:
        prescale = float(sys.argv[3])

    monitor = RateMonitor(mode=mode, prescale=prescale)

    print 'cmos_monitor running in %s mode...' % mode
    while True:
        rec = client.recv()

        if rec:
            if isinstance(rec, ROOT.SNOT.CMOSRate):
                print 'received rates for crate', rec.crate
                crate = rec.crate
                for i in range(len(rec.rates)):
                    slot, channel = get_sc(i)
                    rate = rec.rates[i]
                    monitor.update_channel(crate, slot, channel, rate)
                monitor.update_speaker()
            else:
                print 'bad data received'


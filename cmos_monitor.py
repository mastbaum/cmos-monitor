#!/usr/bin/env python

import sys
import avalanche
import audiere
import numpy
import ROOT

ROOT.gROOT.ProcessLine(".L CMOSRates.h+")

CRATES = 19
SLOTS = 16
CHANNELS = 32

def get_sc(n):
    slot = (n & 0x1e0) >> 5
    chan = n & 0x1f
    return slot, chan

class RateSpeaker:
    def __init__(self, freq, prescale):
        self.freq = freq
        self.prescale = prescale
    def start(self, device, freq, prescale=None):
        self.freq = freq
        if prescale:
            self.prescale = prescale
        if hasattr(self, 'stream'):
            self.stop()
        self.stream = device.create_tone(self.freq / self.prescale)
        self.stream.play()
    def stop(self):
        if hasattr(self, 'stream'):
            self.stream.stop()

VRateSpeaker = numpy.vectorize(RateSpeaker)

class RateMonitor:
    def __init__(self, prescale=None):
        devices = audiere.get_devices()
        if 'oss' in devices:
            self.device = audiere.open_device('oss')
        elif 'alsa' in devices:
            self.device = audiere.open_device('alsa')
        else:
            raise RuntimeError('no suitable audio device found!')

        freq = numpy.zeros((CRATES, SLOTS, CHANNELS), dtype=numpy.float32)
        if not prescale:
            prescale = numpy.ones_like(freq)

        self.channels = numpy.empty((CRATES, SLOTS, CHANNELS), dtype=object)
        self.channels[:,:,:] = VRateSpeaker(freq, prescale)

    def update_channel(self, crate, slot, channel, freq, prescale=None):
        if freq == 0:
            self.channels[crate][slot][channel].stop()
        else:
            self.channels[crate][slot][channel].start(self.device, freq, prescale)

if __name__ == '__main__':
    client = avalanche.Client(sys.argv[1])
    monitor = RateMonitor()

    print 'cmos_monitor running...'
    while True:
        rec = client.recv_object(ROOT.CMOSRates.Class())

        if rec:
            print 'received rates for crate', rec.crate
            crate = rec.crate
            for i in range(len(rec.rates)):
                slot, channel = get_sc(i)
                rate = rec.rates[i]
                monitor.update_channel(crate, slot, channel, rate)
        else:
            print 'error deserializing message data'


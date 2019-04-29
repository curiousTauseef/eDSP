import unittest
import pedsp.io as io
import numpy as np
import utility
import random
import taglib
import os.path
from pysndfile import *


class TestIOMethods(unittest.TestCase):

    def testing_metadata(self):
        repository, files = utility.get_list_test_files()
        for filename in files:
            f = os.path.join(repository, filename)
            metadata = io.MetaDataReader(str(f))
            tfile = taglib.File(f)

            if 'GENRE' in tfile.tags:
                self.assertEqual(metadata.genre(), tfile.tags['GENRE'][0])
            else:
                self.assertTrue(not metadata.genre())

            if 'TITLE' in tfile.tags and tfile.tags['TITLE'][0] != 'untitled':
                self.assertEqual(metadata.title(), tfile.tags['TITLE'][0])
            else:
                self.assertTrue(not metadata.title())

            if 'ARTIST' in tfile.tags:
                self.assertEqual(metadata.artist(), tfile.tags['ARTIST'][0])
            else:
                self.assertTrue(not metadata.artist())

            if 'ALBUM' in tfile.tags:
                self.assertEqual(metadata.album(), tfile.tags['ALBUM'][0])
            else:
                self.assertTrue(not metadata.album())

            if 'TRACK' in tfile.tags:
                self.assertEqual(metadata.track(), tfile.tags['TRACK'][0])
            else:
                self.assertTrue(metadata.track() == 0)

            if 'YEAR' in tfile.tags:
                self.assertEqual(metadata.year(), tfile.tags['YEAR'][0])
            else:
                self.assertTrue(metadata.year() == 0)

    def test_decoder_open_file(self):
        repository, files = utility.get_list_test_files()
        for filename in files:
            f = os.path.join(repository, filename)
            decoder = io.Decoder()
            decoder.open(str(f))
            sndfile = PySndfile(f)
            self.assertEqual(sndfile.channels(), decoder.channels())
            self.assertEqual(sndfile.samplerate(), decoder.samplerate())

    def test_decoder_read_all_data(self):
        repository, files = utility.get_list_test_files()
        for filename in files:
            f = os.path.join(repository, filename)
            decoder = io.Decoder()
            decoder.open(str(f))

            sndfile = PySndfile(f)
            frames = sndfile.read_frames(dtype=np.float32)
            self.assertEqual(decoder.frames(), frames.shape[0])

            data, data_count = decoder.read(frames.size)
            data = data.reshape(frames.shape)

            self.assertEqual(data_count, frames.size)
            np.testing.assert_array_almost_equal(frames, data)

    def test_decoder_seek(self):
        repository, files = utility.get_list_test_files()
        for filename in files:
            f = os.path.join(repository, filename)
            decoder = io.Decoder()
            decoder.open(str(f))

            sndfile = PySndfile(f)
            self.assertEqual(decoder.seekable(), sndfile.seekable())

            index = random.randint(0, decoder.frames() - 1)

            seek_counter_pedsp = decoder.seek(index)
            seek_counter_sndfile = sndfile.seek(index)
            self.assertEqual(seek_counter_pedsp, seek_counter_sndfile)

            remaining = decoder.frames() - seek_counter_pedsp
            frames = sndfile.read_frames(remaining, dtype=np.float32)
            data, data_count = decoder.read(frames.size)
            data = data.reshape(frames.shape)

            self.assertEqual(data_count, frames.size)
            np.testing.assert_array_almost_equal(frames, data)
#ifndef AUDIO_PROCESSOR_H
#define AUIDO_PROCESSOR_H

#include <string>
#include <vector>

namespace audio_processor {

    /**
     * @struct AudioData
     * Represents decoded PCM audio data extracted from an .mp3 file.
     */
    struct AudioData {
        int sampleRate;                 // The number of samples per second (e.g., 44100 Hz)
        int channels;                   // Number of audio channels (1 = mono, 2 = stereo)
        std::vector<float> samples;     // Interleaved PCM samples in normalized float format [-1.0, 1-0]
    };

    /**
     * Loads and decodes an MP3 file using the libmpg123 library.  
     * 
     * @param filepath
     *      The absolute or relative path to the .mp3 file to be decoded.  
     * 
     * @return 
     *      An AudioData struct containing:
     *          - The audio sample rate (Hz)
     *          - The number of channels
     *          - A vector of normalized PCM samples (as floats in the range -1.0 to 1.0)
     * 
     * @throws std::runtime_error
     *      If the file cannot be opened, is invalid, or decoding fails.  
     * 
     * @note
     *      this function is intended to be used as a preprocessing step before performing FFT analysis and beat detection.  
     */
    AudioData load_and_process_mp3(const std::string& filePath);
}


#endif
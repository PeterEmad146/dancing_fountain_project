#include "audio_processor.h"

#include <mpg123.h>
#include <stdexcept>
#include <iostream>
#include <cstring>

namespace audio_processor {

    AudioData load_and_process_mp3(const std::string& filePath) {
        mpg123_handle* mh = nullptr;
        int err = MPG123_OK;
        int channels = 0, encoding = 0;
        long rate = 0;

        // Initialize the mpg123 library
        if (mpg123_init() != MPG123_OK) {
            throw std::runtime_error("Failed to initialize mpg123 library.");
        }

        // Create a new decoder handle
        mh = mpg123_new(nullptr, &err);
        if(!mh) {
            throw std::runtime_error("Failed to create mpg123 handle: " + std::string(mpg123_plain_strerror(err)));
        }

        // Open the MP3 file
        if (mpg123_open(mh, filePath.c_str()) != MPG123_OK) {
            mpg123_delete(mh);
            throw std::runtime_error("Failed to open MP3 file: " + filePath);
        }

        // Get audio format information (rate, channels, encoding)
        if (mpg123_getformat(mh, &rate, &channels, &encoding) != MPG123_OK) { 
            mpg123_close(mh);
            mpg123_delete(mh);
            throw std::runtime_error("Failed to get audio format from file: " + filePath);
        }

        // Force output format to 16-bit signed PCM
        mpg123_format_none(mh);
        mpg123_format(mh, rate, channels, MPG123_ENC_SIGNED_16);

        std::vector<float> floatSamples;
        const size_t bufferSize = 8192;
        unsigned char buffer[bufferSize];
        size_t bytesRead = 0;

        // Read and decode the audio stream
        while (mpg123_read(mh, buffer, bufferSize, &bytesRead) == MPG123_OK){
            // Each sample is 2 bytes (int16_t), interleaved
            int16_t* samples = reinterpret_cast<int16_t*>(buffer);
            size_t sampleCount = bytesRead / sizeof(int16_t);

            // Normalize and store as float (-1.0 to 1.0)
            for (size_t i = 0; i < sampleCount; ++i) {
                floatSamples.push_back(samples[i] / 32768.0f);
            }
        }

        // Clean up mpg123 resources
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();

        // Return decoded and normalized audio data
        return AudioData {
            static_cast<int>(rate),
            channels,
            std::move(floatSamples)
        };
    }
}
// Author: Peter Emad

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>

namespace file_utils {

    /**
     * @brief Prompts the user to enter the path to a .mp3 file and validate the file. 
     * 
     * Validation includes:
     *  - Existence of the file
     *  - File format is .mp3
     *  - File size is less than or equal to 5MB
     *  - Valid MP3 header (ID3v1 or ID3v2)
     * 
     * If the file passes all checks, its metadata (if available) will be displayed.
     * @return std::string - A validated file path. 
     */
    std::string prompt_and_validate_mp3();

    /**
     * @brief Validates a given .mp3 file path for format, size, and content.
     * 
     * Checks performed:  
     *  - Path exists and is a regular file
     *  - File has a .mp3 extension
     *  - File size is less than or equal to 5MB
     *  - File contains a valid MP3 header (ID3v1 or ID3v2)
     * 
     * @param filePath - Path to the .mp3 file
     * @return true if valid, false otherwise
     */
    bool is_valid_mp3_file (const std::string& filePath);

    /**
     * @brief Reads and displays basic ID3v1 metadata from a .mp3 file
     * 
     * Metadata fields:
     *  - Title
     *  - Artist
     *  - Album
     *  - Year
     * 
     * Note: ID3v1 tags are located at the last 128 bytes of an MP3 file
     * This function will display "No metadata found" if the tag is missing
     * 
     * @param filePath - Path to the .mp3 file
     */
    void read_mp3_metadata (const std::string& filePath);

    /**
     * @brief Verifies the presence of a valid MP3 header in the file.is_valid_mp3_file
     * 
     * This function checks:
     *  - "ID3" tag at the beginning of the file (ID3v2 format)
     *  - "TAG" marker at the end of the file (ID3v1 format)
     * 
     * @param filePath - Path to the .mp3 file
     * @return true if a vlid MP3 tag is foundt, false otherwise
     */
    bool verify_mp3_header (const std::string& filePath);

}

#endif // FILE_UTILS_H
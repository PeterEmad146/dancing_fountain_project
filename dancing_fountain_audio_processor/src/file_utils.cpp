#include "file_utils.h"

#include <iostream>
#include <fstream>      // For file reading
#include <filesystem>   // For file system operations
#include <iomanip>      // For formatting metadata display

namespace fs = std::filesystem;     // Alias for convenience

namespace file_utils {

    bool verify_mp3_header (const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if(!file) {
            std::cerr << "Error: Unable to open file for header check.\n";
            return false;
        }

        // Check beginning of file for ID3v2
        char header[3];
        file.read(header, 3);
        if (file.gcount() == 3 && header[0] == 'I' && header[1] == 'D' && header[2] == '3') {
            std::cout << "Valid MP3 file: IDv2 tag detected.\n";
            return true;
        }

        // Seek to last 128 bytes and check for ID3v1 "TAG"
        file.seekg(-128, std::ios::end);
        char tag[3];
        file.read(tag, 3);
        if (file.gcount() == 3 && tag[0] == 'T' && tag[1] == 'A' && tag[2] == 'G') {
            std::cout << "Valid MP3 file: ID3v1 tag detected.\n";
            return true;
        }

        std::cerr << "Warning: No valid MP3 header (ID3v1/v2) found.\n";
        return false;
    }

    void read_mp3_metadata (const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            std::cerr << "Error: Unable to open file for metadata reading.\n";
            return;
        }

        file.seekg(-128, std::ios::end);    // Seek to where ID3v1 tag begins
        char tag[3];
        file.read(tag, 3);

        if (file.gcount() < 3 || tag[0] != 'T' || tag[1] != 'A' || tag[2] != 'G') {
            std::cout << "No ID3v1 metadata found.\n";
            return;
        }

        // Allocate buffers for metadata fields (with null terminators)
        char title [31] = {0}, artist[31] = {0}, album[31] = {0}, year[5]={0};

        file.read(title, 30);       // Read 30 bytes for title
        file.read(artist, 30);      // Read 30 bytes for artist
        file.read(album, 30);       // Read 30 bytes for album
        file.read(year, 4);         // Read 4 bytes for year

        // Display metadata to the user
        std::cout << "\n🎵 MP3 Metadata (ID3v1):\n";
        std::cout << "  Title : " << title << "\n";
        std::cout << "  Artist: " << artist << "\n";
        std::cout << "  Album : " << album << "\n";
        std::cout << "  Year  : " << year << "\n";
    }

    bool is_valid_mp3_file (const std::string& filePath) {
        fs::path path(filePath);

        // Check if path exists
        if (!fs::exists(path)) {
            std::cerr << "Error: File does not exist.\n";
            return false;
        }

        // Ensure it's a regular file (not a folder or symlink)
        if (!fs::is_regular_file(path)) {
            std::cerr << "Error: path is not a regular file.\n";
            return false;
        }

        // Check file extension is .mp3 (case-sensitive)
        if (path.extension() != ".mp3") {
            std::cerr << "Error: File is not an .mp3 format.\n";
            return false;
        }

        // Maximum allowed file size = 5MB (5 * 1024 * 1024 bytes)
        constexpr std::uintmax_t maxSize = 5 * 1024 * 1024;
        if (fs::file_size(path)>maxSize) {
            std::cerr << "Error: File size exceeds 5MB.\n";
            return false;
        }

        // Check file content to ensure it's really an MP3
        if (!verify_mp3_header(filePath)) {
            std::cerr << "Error: File content does not appear to be a valid MP3.\n";
            return false;
        }

        // Extract and display metadata (if header found)
        read_mp3_metadata(filePath);
        return true;
    }

    std::string prompt_and_validate_mp3() {
        std::string inputPath;

        while (true) {
            std::cout << "Enter the path to the .mp3 file (max 5MB): ";
            std::getline(std::cin, inputPath);  // Read full line to allow spaces

            // Validate the input file path
            if (is_valid_mp3_file(inputPath)) {
                std::cout << "✅ File validated successfully.\n";
                break;
            } else {
                std::cout << "❌ Invalid input. Please try again.\n";
            }
        }

        return inputPath;
    }
}
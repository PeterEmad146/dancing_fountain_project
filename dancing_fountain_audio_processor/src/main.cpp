#include <iostream>
#include "file_utils.h"  // Include the header for file validation functions

int main() {
    std::cout << "🎧 MP3 File Validator\n";
    std::cout << "----------------------\n";

    // Prompt the user for an MP3 file path and validate it
    std::string validMP3Path = file_utils::prompt_and_validate_mp3();

    // At this point, the file has been verified
    std::cout << "\n📂 File Ready for Use: " << validMP3Path << "\n";

    // You can now proceed to do something with the valid MP3 file
    // e.g., add to a playlist, copy to another directory, etc.

    return 0;
}

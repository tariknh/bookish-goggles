#pragma once
#include <iostream>
#include <streambuf>
#include <string>
#include <memory>   // For std::unique_ptr

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdio>   // For stdout, fwrite
#endif

/**
 * @class Utf8ConsoleStreambuf
 * @brief A custom streambuf that captures output to std::cout
 * and forces it to be written as UTF-8 to the console, even on Windows.
 */
class Utf8ConsoleStreambuf : public std::streambuf {
public:
	// Destructor: Ensure the buffer is flushed
	~Utf8ConsoleStreambuf() override {
		sync(); // Flush the buffer on destruction
	}

protected:
	/**
	 * @brief Called when the buffer needs to be flushed (synchronized).
	 * For example, when using std::endl or when the buffer is full.
	 */
	int sync() override {
		if (buffer_.empty()) {
			return 0; // Nothing to do
		}

#ifdef _WIN32
		// --- Windows-specific solution ---
		// 1. Convert our internal UTF-8 buffer to a wide (UTF-16) string
		std::wstring wbuffer = ConvertUtf8ToWide(buffer_);

		// 2. Get the standard output handle
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (console_handle == INVALID_HANDLE_VALUE) {
			return -1; // Error
		}

		// 3. Write the wide string directly to the console API
		DWORD chars_written = 0;
		WriteConsoleW(console_handle,
			wbuffer.c_str(),
			static_cast<DWORD>(wbuffer.length()),
			&chars_written,
			nullptr);
#else
		// --- Linux/macOS-løsning ---
		// Bare skriv den rå UTF-8-bufferen til standard output
		std::fwrite(buffer_.c_str(), 1, buffer_.length(), stdout);
		std::fflush(stdout);
#endif

		buffer_.clear(); // Clear our internal buffer
		return 0; // Success
	}

	/**
	 * @brief Called when a character "overflows", i.e., is sent to the buffer.
	 */
	int_type overflow(int_type c = traits_type::eof()) override {
		if (c != traits_type::eof()) {
			char ch = static_cast<char>(c);

			// Append the character to our internal buffer
			buffer_ += ch;

			// Flush the buffer automatically on every newline
			if (ch == '\n') {
				sync();
			}
		}
		return c; // Return the character
	}

private:
	std::string buffer_; // Our internal UTF-8 buffer

#ifdef _WIN32
	// Helper function to convert UTF-8 (std::string) to UTF-16 (std::wstring)
	std::wstring ConvertUtf8ToWide(const std::string& utf8_str) {
		if (utf8_str.empty()) {
			return std::wstring();
		}

		// Calculate the required buffer size for the wide string
		int wide_len = MultiByteToWideChar(
			CP_UTF8,       // Source code page (UTF-8)
			0,             // Flags
			utf8_str.c_str(), // Source string
			-1,            // Assume null-terminated
			nullptr,       // No buffer (we just want the size)
			0              // Size 0
		);

		if (wide_len == 0) {
			return std::wstring(); // Error
		}

		// Allocate buffer and perform the conversion
		std::wstring wide_str(wide_len, 0);
		MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wide_len);

		// Remove the extra null terminator that MultiByteToWideChar includes
		wide_str.resize(static_cast<std::wstring::size_type>(wide_len) - 1);

		return wide_str;
	}
#endif
};
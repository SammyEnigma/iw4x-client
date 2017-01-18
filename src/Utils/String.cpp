#include "STDInclude.hpp"
#ifndef DISABLE_BASE128
#include "base128.h"
#endif

#define VA_BUFFER_COUNT		32
#define VA_BUFFER_SIZE		65536

namespace Utils
{
	namespace String
	{
		const char *VA(const char *fmt, ...)
		{
			static char g_vaBuffer[VA_BUFFER_COUNT][VA_BUFFER_SIZE];
			static int g_vaNextBufferIndex = 0;

			va_list ap;
			va_start(ap, fmt);
			char* dest = g_vaBuffer[g_vaNextBufferIndex];
			vsnprintf_s(g_vaBuffer[g_vaNextBufferIndex], VA_BUFFER_SIZE, fmt, ap);
			g_vaNextBufferIndex = (g_vaNextBufferIndex + 1) % VA_BUFFER_COUNT;
			va_end(ap);
			return dest;
		}

		std::string ToLower(std::string input)
		{
			std::transform(input.begin(), input.end(), input.begin(), ::tolower);
			return input;
		}

		std::string ToUpper(std::string input)
		{
			std::transform(input.begin(), input.end(), input.begin(), ::toupper);
			return input;
		}

		bool EndsWith(std::string haystack, std::string needle)
		{
			return (strstr(haystack.data(), needle.data()) == (haystack.data() + haystack.size() - needle.size()));
		}

		std::string DumpHex(std::string data, std::string separator)
		{
			std::string result;

			for (unsigned int i = 0; i < data.size(); ++i)
			{
				if (i > 0)
				{
					result.append(separator);
				}

				result.append(Utils::String::VA("%02X", data[i] & 0xFF));
			}

			return result;
		}

		std::string XOR(std::string str, char value)
		{
			for (unsigned int i = 0; i < str.size(); ++i)
			{
				str[i] ^= value;
			}

			return str;
		}

		std::vector<std::string> Explode(const std::string& str, char delim)
		{
			std::vector<std::string> result;
			std::istringstream iss(str);

			for (std::string token; std::getline(iss, token, delim);)
			{
				std::string _entry = std::move(token);

				// Remove trailing 0x0 bytes
				while (_entry.size() && !_entry.back())
				{
					_entry = _entry.substr(0, _entry.size() - 1);
				}

				result.push_back(_entry);
			}

			return result;
		}

		void Replace(std::string &string, std::string find, std::string replace)
		{
			size_t nPos = 0;

			while ((nPos = string.find(find, nPos)) != std::string::npos)
			{
				string = string.replace(nPos, find.length(), replace);
				nPos += replace.length();
			}
		}

		bool StartsWith(std::string haystack, std::string needle)
		{
			return (haystack.size() >= needle.size() && !strncmp(needle.data(), haystack.data(), needle.size()));
		}

		int IsSpace(int c)
		{
			if (c < -1) return 0;
			return _isspace_l(c, nullptr);
		}

		// trim from start
		std::string &LTrim(std::string &s)
		{
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(IsSpace))));
			return s;
		}

		// trim from end
		std::string &RTrim(std::string &s)
		{
			s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(IsSpace))).base(), s.end());
			return s;
		}

		// trim from both ends
		std::string &Trim(std::string &s)
		{
			return LTrim(RTrim(s));
		}

		std::string FormatTimeSpan(int milliseconds)
		{
			int secondsTotal = milliseconds / 1000;
			int seconds = secondsTotal % 60;
			int minutesTotal = secondsTotal / 60;
			int minutes = minutesTotal % 60;
			int hoursTotal = minutesTotal / 60;

			return Utils::String::VA("%02d:%02d:%02d", hoursTotal, minutes, seconds);
		}

		std::string FormatBandwidth(size_t bytes, int milliseconds)
		{
			static char* sizes[] = 
			{
				"B",
				"KB",
				"MB",
				"GB",
				"TB"
			};

			double bytesPerSecond = (1000.0 / milliseconds) * bytes;

			int i = 0;
			for (i = 0; bytesPerSecond > 1000 && i < ARRAY_SIZE(sizes); ++i) // 1024 or 1000?
			{
				bytesPerSecond /= 1000;
			}

			return Utils::String::VA("%.2f %s/s", static_cast<float>(bytesPerSecond), sizes[i]);
		}

		// Encodes a given string in Base64
		std::string EncodeBase64(const char* input, const unsigned long inputSize) 
		{
			unsigned long outlen = long(inputSize + (inputSize / 3.0) + 16);
			unsigned char* outbuf = new unsigned char[outlen]; //Reserve output memory
			base64_encode(reinterpret_cast<unsigned char*>(const_cast<char*>(input)), inputSize, outbuf, &outlen);
			std::string ret((char*)outbuf, outlen);
			delete[] outbuf;
			return ret;
		}

		// Encodes a given string in Base64
		std::string EncodeBase64(const std::string& input) 
		{
			return EncodeBase64(input.data(), input.size());
		}

#ifndef DISABLE_BASE128
		// Encodes a given string in Base128
		std::string EncodeBase128(const std::string& input) 
		{
			base128 encoder;

			void* inbuffer = const_cast<char*>(input.data());
			char* buffer = encoder.encode(inbuffer, input.size());
			/*
			Interesting to see that the buffer returned by the encoder is not a standalone string copy
			but instead is a pointer to the internal "encoded" field of the encoder. So if you deinitialize
			the encoder that string will probably become garbage.
			*/
			std::string retval(buffer);
			return retval;
		}
#endif

		// Generates a UUID and returns the string representation of it
		std::string GenerateUUIDString() 
		{
			// Generate UUID data
			UUID uuid;
			if (!UuidCreate(&uuid))
			{
				// Convert to string representation
				char* strdata = nullptr;
				if (!UuidToStringA(&uuid, reinterpret_cast<RPC_CSTR*>(&strdata)))
				{
					return std::string(strdata);
				}
			}

			return "";
		}
	}
}

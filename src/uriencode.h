//
//  urlencode.h
//  ofxRemoteUIClientOSX
//
//  Created by Hansi on 13.06.13.
//
//
// based on http://www.codeguru.com/cpp/cpp/algorithms/strings/article.php/c12759/URI-Encoding-and-Decoding.htm

#ifndef ofxRemoteUIClientOSX_urlencode_h
#define ofxRemoteUIClientOSX_urlencode_h

// hansi: hextable from 
// http://stackoverflow.com/questions/10324/how-can-i-convert-a-hexadecimal-number-to-base-10-efficiently-in-c
static const long HEX2DEC[] = {
	[0 ... 255] = -1, // bit aligned access into this table is considerably
	['0'] = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, // faster for most modern processors,
	['A'] = 10, 11, 12, 13, 14, 15,       // for the space conscious, reduce to
	['a'] = 10, 11, 12, 13, 14, 15        // signed char.
};

// http://en.wikipedia.org/wiki/Percent-encoding#Types_of_URI_characters
static bool SAFE[256] = {
	[0 ... 255] = false,
	['a'...'z'] = true,
	['A'...'Z'] = true,
	['0'...'9'] = true,
	['.'] = true
};

std::string UriEncode(const std::string & sSrc);
std::string UriEncode(const std::string & sSrc)
{
	
	
	const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
	const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
	const int SRC_LEN = sSrc.length();
	unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
	unsigned char * pEnd = pStart;
	const unsigned char * const SRC_END = pSrc + SRC_LEN;
	
	for (; pSrc < SRC_END; ++pSrc)
	{
		if (SAFE[*pSrc])
			*pEnd++ = *pSrc;
		else
		{
			// escape this char
			*pEnd++ = '%';
			*pEnd++ = DEC2HEX[*pSrc >> 4];
			*pEnd++ = DEC2HEX[*pSrc & 0x0F];
		}
	}
	
	std::string sResult((char *)pStart, (char *)pEnd);
	delete [] pStart;
	return sResult;
}


std::string UriDecode(const std::string & sSrc); 
std::string UriDecode(const std::string & sSrc)
{
	// Note from RFC1630: "Sequences which start with a percent
	// sign but are not followed by two hexadecimal characters
	// (0-9, A-F) are reserved for future extension"
	
	const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
	const int SRC_LEN = sSrc.length();
	const unsigned char * const SRC_END = pSrc + SRC_LEN;
	// last decodable '%'
	const unsigned char * const SRC_LAST_DEC = SRC_END - 2;
	
	char * const pStart = new char[SRC_LEN];
	char * pEnd = pStart;
	
	while (pSrc < SRC_LAST_DEC)
	{
		if (*pSrc == '%')
		{
			char dec1, dec2;
			if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
				&& -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
			{
				*pEnd++ = (dec1 << 4) + dec2;
				pSrc += 3;
				continue;
			}
		}
		
		*pEnd++ = *pSrc++;
	}
	
	// the last 2- chars
	while (pSrc < SRC_END)
		*pEnd++ = *pSrc++;
	
	std::string sResult(pStart, pEnd);
	delete [] pStart;
	return sResult;
}

#endif
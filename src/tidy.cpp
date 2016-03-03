#include "crawl/tidy.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <boost/format.hpp>

#include <tidy.h>
#include <tidybuffio.h>
#include <iconv.h>

namespace crawl {
  class Tidy::TidyPrivate
  {
  public:
    TidyPrivate(Tidy *p_parent = NULL)
      : parent(p_parent)
      , tdoc(tidyCreate())
      , rc(0)
    {
      //output = (TidyBuffer*)malloc(8192 * sizeof(TidyBuffer));
      //tidyBufAlloc(&output, 181920);
      conv = iconv_open("UTF-8", "SJIS");
    }
    ~TidyPrivate() {
      iconv_close(conv);
      tidyRelease( tdoc );
    }
    std::string saveAsUTF8(const std::string &file) {
      std::ifstream ifs(file.c_str());
      std::ofstream ofs((boost::format("/dev/shm/%d") % getpid()).str());
      std::string buf;

      if (ifs.fail())
        return "";

      while (std::getline(ifs, buf)) {
        ofs << buf;
      }
      ofs.close();

    }

    std::string sjis2utf8(const std::string &input) {
      std::vector<char> in_buf(input.begin(), input.end());
      char* src_ptr = &in_buf[0];
      size_t src_size = input.size();
      int buf_size = 1024;

      std::vector<char> buf(buf_size);
      std::string dst;
      while (0 < src_size) {
          char* dst_ptr = &buf[0];
          size_t dst_size = buf.size();
          size_t res = ::iconv(conv, &src_ptr, &src_size, &dst_ptr, &dst_size);
          dst.append(&buf[0], buf.size() - dst_size);
      }
      return dst;
    }

    Tidy *parent;
    TidyDoc tdoc;
    int rc;
    iconv_t conv;
  };

  Tidy::Tidy()
    : d(new TidyPrivate(this))
  {
  }

  Tidy::~Tidy() {
    delete d;
  }

  std::string Tidy::convert(const boost::filesystem::path &htmlFile, const std::string &encoding) const {
    bool ok = tidyOptSetBool( d->tdoc, TidyXmlOut, yes );
    ok &= tidyOptSetBool(d->tdoc, TidyQuiet, yes);
    ok &= tidyOptSetBool(d->tdoc, TidyQuoteNbsp, no);
    ok &= tidyOptSetBool(d->tdoc, TidyXmlDecl, yes);
    //tidyOptSetBool(d->tdoc, TidyHtmlOut, yes); // don't ns output
    //std::cout << TidyForceOutput << std::endl;
    //std::cout << "hoge" << std::endl;
    ok &= tidyOptSetBool(d->tdoc, TidyForceOutput, yes);
    //tidyOptSetValue(d->tdoc, TidyCharEncoding, encoding.c_str());
    //      tidyOptSetValue(d->tdoc, TidyCharEncoding, "utf8");
    ok &= tidyOptSetValue(d->tdoc, TidyInCharEncoding, encoding.c_str());
    ok &= tidyOptSetValue(d->tdoc, TidyOutCharEncoding, "utf8");
    ok &= tidyOptSetBool(d->tdoc, TidyNumEntities, yes);
    ok &= tidyOptSetBool(d->tdoc, TidyShowWarnings, no);
    ok &= tidyOptSetInt(d->tdoc, TidyDoctypeMode, TidyDoctypeOmit);


    if (!ok)
      return "";


    TidyBuffer output({0});
    d->rc = tidyParseFile( d->tdoc, htmlFile.string().c_str() );           // Parse the input
    d->rc = tidyCleanAndRepair(d->tdoc);               // Tidy it up!
    d->rc = tidySaveBuffer(d->tdoc, &output);          // Pretty Print

    std::string buf;
    std::stringstream sin;
    std::stringstream sout;
    sin << reinterpret_cast<char*>(output.bp);

    bool is_replaced = false;
    const static std::regex re("^<html.*$");
    while (std::getline(sin, buf)) {
      if (!is_replaced) {
        if (std::regex_match(buf, re)) {
          buf = std::regex_replace(buf, re, "<html>");
          is_replaced = true;
        }
      }
      sout << buf << std::endl;
    }
    tidyBufFree( &output );
    return sout.str();
  }
}

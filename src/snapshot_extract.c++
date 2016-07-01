#include "snapshot.h++"
#include "snapshot_internal.h++"
#include "fileno.h++"
#include <vector>

using namespace std;
using namespace boost::filesystem;

namespace snapshot {

  template<typename T>
  void read_value(std::istream& image, T& value) {
    image.read(reinterpret_cast<char*>(&value), sizeof(T));
    cout << "read: " << value << " [" << sizeof(T) << "]" << " " << image.gcount() << std::endl;
  }

  template<>
  void read_value<std::string>(std::istream& image, std::string& content) {
    std::string::size_type size = 0;
    image.read(reinterpret_cast<char*>(&size), sizeof(std::string::size_type));
    cout << "read: " << size << " [" << sizeof(std::string::size_type) << "]" << " " << image.gcount() << std::endl;
    content.resize(size, '\0');
    if(size > 0) {
      image.read(const_cast<char*>(content.data()), size);
      cout << "read: " << content << " [" << size << "]" << " " << image.gcount() << std::endl;
    }
  }

  void read_file(std::ifstream& image, boost::filesystem::path file) {

      // read file size
      size_t filesize = 0;
      read_value(image, filesize);

      // open file
      ofstream stream(file.string().c_str(), ios::binary | ios::trunc);
      stream.exceptions(ifstream::failbit | ifstream::badbit);

      // copy file content
      std::copy_n(
        istreambuf_iterator<char>(image),
        filesize,
        ostreambuf_iterator<char>(stream));

      cout << "read: #file [" << filesize << "]" << " " << image.gcount() << std::endl;
  }

  void read_directory(std::ifstream& image, boost::filesystem::path directory) {

    // create target directory
    create_directories(directory);

    // read children count
    size_t size = 0;
    read_value(image, size);

    // extract children
    for( size_t i = 0; i < size; ++i ) {

      // extract name
      std::string name;
      read_value(image, name);

      // extract type
      NODE_TYPE type = NODE_UNKNOWN;
      read_value(image, type);

      // extract directory
      if(type == NODE_DIRECTORY) {
        read_directory(image, directory / name);
      }
      else
      if(type == NODE_FILE) {
        read_file(image, directory / name);
      }
    }
  }

  void extract(std::ifstream& image, boost::filesystem::path directory) {

    // enable exceptions on image stream
    image.exceptions(ifstream::failbit | ifstream::badbit);

    // read directory
    read_directory(image, directory);
  }

  void extract(boost::filesystem::path image, boost::filesystem::path directory) {

      // open file
      ifstream stream(image.string().c_str(), ios::binary);

      // extract image
      extract(stream, directory);
  }

}

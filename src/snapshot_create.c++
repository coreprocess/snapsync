#include "snapshot.h++"
#include "snapshot_internal.h++"
#include "fileno.h++"
#include <vector>

using namespace std;
using namespace boost::filesystem;

namespace snapshot {

  template<typename T>
  void write_value(const T& value, std::ostream& image) {
    image.write(reinterpret_cast<const char*>(&value), sizeof(T));
    cout << "written: " << value << " [" << sizeof(T) << "]" << std::endl;
  }

  template<>
  void write_value<std::string>(const std::string& content, std::ostream& image) {
    const std::string::size_type size = content.size();
    image.write(reinterpret_cast<const char*>(&size), sizeof(std::string::size_type));
    cout << "written: " << size << " [" << sizeof(std::string::size_type) << "]" << std::endl;
    if(size > 0) {
      image.write(content.data(), size);
      cout << "written: " << content << " [" << size << "]" << std::endl;
    }
  }

  void write_file(boost::filesystem::path file, std::ofstream& image) {

    // write file size
    const size_t filesize = file_size(file);
    write_value(filesize, image);

    // open file
    ifstream stream(file.string().c_str(), ios::binary);
    stream.exceptions(ifstream::failbit | ifstream::badbit);

    // copy content
    std::copy_n(
      istreambuf_iterator<char>(stream),
      filesize,
      ostreambuf_iterator<char>(image));

    cout << "written: #file [" << filesize << "]" << std::endl;
  }

  void write_directory(boost::filesystem::path directory, std::ofstream& image) {

    // load children paths into memory
    vector<path> children;
    copy(directory_iterator(directory), directory_iterator(), back_inserter(children));
    sort(children.begin(), children.end());

    // write size
    write_value(children.size(), image);

    // iterate children
    for(auto child : children) {

      // write name
      write_value(child.filename().string(), image);

      // write type and content
      if(is_directory(child)) {
        write_value(NODE_DIRECTORY, image);
        write_directory(child, image);
      }
      else
      if(is_regular_file(child)) {
        write_value(NODE_FILE, image);
        write_file(child, image);
      }
      else
      if(exists(child)) {
        write_value(NODE_UNKNOWN, image);
      }
      else {
        write_value(NODE_NONE, image);
      }
    }
  }

  void create(boost::filesystem::path directory, std::ofstream& image) {

    // enable exceptions on image stream
    image.exceptions(ifstream::failbit | ifstream::badbit);

    // write directory
    write_directory(directory, image);
  }

  void create(boost::filesystem::path directory, boost::filesystem::path image) {

      // open file
      ofstream stream(image.string().c_str(), ios::binary | ios::trunc);

      // create image
      create(directory, stream);
  }

}

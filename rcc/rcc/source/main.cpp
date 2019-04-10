#include <fstream>
#include <cassert>
#include <iomanip>

// rcc <input> <output> <key>

int main(int argc, char** argv)
{
    assert(argc == 4);

    std::ofstream out(argv[2]);
    out << "#include <cstddef>" << std::endl;
    out << "#include <string>" << std::endl << std::endl;
    out << "const unsigned char data[] = {" << std::endl;

    std::size_t size = 0;
    {
        std::ifstream in(argv[1], std::ios::binary);
        if (!in)
        {
            return 1;
        }

        while (!in.eof())
        {
            size++;
            if (size > 1)
            {
                out << ",";
            }
            else
            {
                out << " ";
            }

            int c = in.get();

            if (c < 0)
            {
                break;
            }

            out << "0x" << std::hex << std::setw(2) << std::setfill('0') << (c & 0xFF);

            if ((size % 20) == 0)
            {
                out << std::endl;
            }
        }
        out << "0x00";
    }

    out << std::dec << std::endl << "};" << std::endl << std::endl;
    out << "extern bool add_resource( const std::string_view, const unsigned char *, const std::size_t );" << std::endl;
    out << "static const bool result = add_resource(\"" << argv[3] << "\", data, sizeof(data));" << std::endl;
    return 0;
}

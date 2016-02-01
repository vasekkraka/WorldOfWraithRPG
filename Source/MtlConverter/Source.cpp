#include <Windows.h>
#include <conio.h>
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp> 

using namespace std;

int main(int argc, char *argv[])
{
	ifstream  mtl_file;
	ofstream  output_file;



	namespace fs = boost::filesystem;

	fs::path targetDir("C:/Wow_MPQ_Extract/world.mpq_mtl");

	fs::directory_iterator it(targetDir), eod;

	int mtl_count = 0;

	BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod))
	{
		if (is_regular_file(p))
		{

			mtl_count++;

			printf("Cesta: %s\n", p.filename().string().c_str());
			mtl_file.open("C:/Wow_MPQ_Extract/world.mpq_mtl/" + p.filename().string());
			output_file.open("C:/Wow_MPQ_Extract/wmo_single/wmo_model/" + p.filename().string());

			std::string line;

			while (!mtl_file.eof())
			{
				line.clear();
				std::getline(mtl_file, line);
				printf("%s\n", line.c_str());

				std::string map_kd;

				map_kd = line.substr(0, 6);
				
				if (!map_kd.compare("map_Kd"))
				{
					std::string text_path = line.substr(7, line.length() - 7);
					printf("'%s'\n", text_path.c_str());

					output_file.write("map_Kd", 6);
					output_file.write(" ..\\wmo_texture\\", 16);
					int back_slash = text_path.find_last_of("\\");
					output_file.write(text_path.substr(back_slash+1).c_str(), text_path.substr(back_slash).length()-4);
					output_file.write("PNG", 3);
					output_file.write("\n", 1);
					
					std::string src_path = "C:\\Wow_MPQ_Extract\\texture.mpq_png_converted\\";
					src_path.append(text_path.substr(0, text_path.length()-3));
					src_path.append("PNG");

					std::string dest_path = "C:\\Wow_MPQ_Extract\\wmo_single\\wmo_texture\\";
					dest_path.append(text_path.substr(back_slash + 1, text_path.substr(back_slash).length() - 4));
					dest_path.append("PNG");

					printf("====> '%s'", src_path.c_str());
					printf("\n\n====> '%s'", dest_path.c_str());

					if (fs::exists(src_path.c_str()))
					{
						fs::copy_file(src_path.c_str(), dest_path.c_str(), fs::copy_option::overwrite_if_exists);
					}
				}
				else
				{
					output_file.write(line.c_str(), line.length());
					output_file.write("\n", 1);
				}
				map_kd.clear();
			}
			
			mtl_file.close();
			output_file.close();

			printf("============================== New File ======================\n");

			//getchar();
			
		}
	}

	printf("\n\nTotal MTL count: %i", mtl_count);

	getchar();

	return 0;
}
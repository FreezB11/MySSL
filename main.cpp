#include "0x.h"
#include "utils.h"
#include "Img.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>

int main() {


    //arkn::arkn(45, 6, 6);

    Img test("test.png");
    std::cout << test.width << std::endl;
    std::cout << test.height << std::endl;
    std::cout << test.channels << std::endl;

    //test.convertToGrayscale();
    test.write_to_file("pixel_data.txt");
    //test.save("img.png");
    test.free_sample();

    return 0;
}
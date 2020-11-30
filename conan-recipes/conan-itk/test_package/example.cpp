#include "itkImage.h"
#include <iostream>

int
main()
{
    itk::Image<unsigned short, 3>::Pointer image = itk::Image<unsigned short, 3>::New();

    std::cout << "ITK Hello World !" << std::endl;

    return EXIT_SUCCESS;
}
